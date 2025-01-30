#include <Arduino.h>
#include "display.h"
#include "ota.h"
#include "ir.h"
#include "weapon.h"

const char* ssid = "KravecNET";
const char* password = "17931793";

// Makro PISKOT_NUMBER defined in platformio.ini
#ifndef PISKOT_NUMBER
#endif

#define MULTI_BTN 16   // D0
#define FIRE_BTN 14    // D5
#define RELOAD_BTN 12  // D6

// VARIABLES
// Player variables
int playerNumber = PISKOT_NUMBER; 
int teamNumber = PISKOT_NUMBER;
int actualHealth;
// Weapon varibles
Weapon pistol = {"Pistol", 7, 7, 1, 3000};  // FEAT: WEAPONS - definition of other guns here
Weapon currentWeapon = pistol;

// Game variables
enum GameState {
  OTA,
  WELCOME,
  GAME,
  GAMEOVER
};
GameState gameState;

// Helper variables
bool lastFireBtnState;
bool lastReloadBtnState;
bool reloading = false;
unsigned long reloadStartTime = 0;

// METHODS
// Main methods
void gameVariablesInit();
void handleGameLogic();

// Screen methods
void welcomeScreen();
void gameScreen();
void gameOverScreen();

// Game logic methods
void fireHandle();
void reloadHandle();
void startReloading();
void decreaseHealth(int damage);

// Helper methods
bool fireBtnPressed();
bool reloadBtnPressed();

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(MULTI_BTN, INPUT_PULLUP);   // Multi button (OTA, Team, Weapon)
  pinMode(FIRE_BTN, INPUT_PULLUP);    // Fire button
  pinMode(RELOAD_BTN, INPUT_PULLUP);  // Reload button
  displayInit();

  if (digitalRead(MULTI_BTN) == HIGH) {
    irInit(decreaseHealth);
    gameVariablesInit();
    welcomeScreen();
    gameState = WELCOME;
  } else {
    otaInit(ssid, password);
    gameState = OTA;
  }
}
void loop() {
  switch (gameState) {
    case OTA: {
      otaHandle();
      break;
    }
    case WELCOME: {
      if (fireBtnPressed()) {
        gameVariablesInit();
        gameScreen();
        enableIr();
        gameState = GAME;
      }
      break;
    }
    case GAME: {
      if (actualHealth <= 0) {
        gameOverScreen();
        disableIr();
        gameState = GAMEOVER;
      } else {
        handleGameLogic();
      }
      break;
    }
    case GAMEOVER: {
      if (fireBtnPressed()) {
        welcomeScreen();
        gameState = WELCOME;
      }
      break;
    }
    default: break;
  }
}

// Main methods
void gameVariablesInit(){
  lastFireBtnState = digitalRead(FIRE_BTN);
  lastReloadBtnState = digitalRead(RELOAD_BTN);
  actualHealth = 5;
  currentWeapon = pistol;
}
void handleGameLogic() {
  hitHandle(playerNumber, teamNumber, actualHealth);  
  if (reloading) {
    reloadHandle();
  } else {
    if (reloadBtnPressed() && currentWeapon.currentAmmo < currentWeapon.maxAmmo) {
      startReloading();
    } else {
      fireHandle();
    }
  }
}

// Screen methods
void welcomeScreen(){
  clearDisplay();
  writeText(String(teamNumber), 1, 122, 0);
  drawMakcen(52, 14);
  writeText("PISKOT", 3, 12, 22);
  writeText("START", 1, 98, 56);
}
void gameScreen(){
  clearDisplay();
  writeText(String(teamNumber), 1, 122, 0);
  drawLives(actualHealth);
  drawAmmo(currentWeapon);
  drawWeaponInfo(currentWeapon);
}
void gameOverScreen(){
  clearDisplay();
  writeText("KONIEC", 3, 12, 22);
  writeText("ZNOVA", 1, 98, 56);
}

// Game logic methods
void fireHandle(){
  if(fireBtnPressed()){
    if(currentWeapon.currentAmmo > 0){
      sendIRData(playerNumber, teamNumber, currentWeapon.damage);
      currentWeapon.currentAmmo --;
      drawAmmo(currentWeapon);
    }
    else{
      // FEAT: AUDIO - no ammo handler (sound of empty gun here)
    }
  }
}
void startReloading(){
  reloading = true;
  reloadStartTime = millis();
  clearDisplay("AMMO");
  writeText("Nabijam...", 1, 25, 30);
}
void reloadHandle(){
  if (millis() - reloadStartTime >= currentWeapon.reloadTime) {
    currentWeapon.currentAmmo = 7;
    reloading = false;
    drawAmmo(currentWeapon);
  } else{
    drawReloadingAnimation(reloadStartTime, currentWeapon.reloadTime);
  }
};
void decreaseHealth(int damage){
  int newHealth = actualHealth - damage;
  if(newHealth <= 0){
    actualHealth = 0;
  } else{
    actualHealth = newHealth;
  }
  drawLives(actualHealth);
}

// Helper methods
bool fireBtnPressed(){
  bool fireBtnState = digitalRead(FIRE_BTN);
  if (fireBtnState == LOW && lastFireBtnState == HIGH) {
    lastFireBtnState = fireBtnState;
    delay(50); // Anti-bounce delay
    return true;
  }
  lastFireBtnState = fireBtnState;
  delay(50); // Anti-bounce delay
  return false;
}
bool reloadBtnPressed(){
  bool reloadBtnState = digitalRead(RELOAD_BTN);
  if (reloadBtnState == LOW && lastReloadBtnState == HIGH) {
    lastFireBtnState = reloadBtnState;
    delay(50); // Anti-bounce delay
    return true;
  }
  lastReloadBtnState = reloadBtnState;
  delay(50); // Anti-bounce delay
  return false;
}