#include <Arduino.h>
#include "display.h"
#include "ota.h"
#include "ir.h"

const char* ssid = "KravecNET";
const char* password = "17931793";

// Makro PISKOT_NUMBER defined in platformio.ini
#ifndef PISKOT_NUMBER
#endif

// Player variables
int playerNumber = PISKOT_NUMBER; 
int teamNumber = PISKOT_NUMBER;
int choosedWeaponDamage;
int actualHealth;
int actualAmmo;

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
bool reloading = false;
unsigned long reloadStartTime = 0;

// Main methods
void gameVariablesInit();
void handleGameLogic();

// Screen methods
void welcomeScreen();
void updateGameScreen();
void gameOverScreen();

// Game logic methods
void fireHandle();
void reloadGun();
void decreaseHealth(int damage);

// Helper methods
bool fireBtnPressed();

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(16, INPUT_PULLUP);    // OTA button
  pinMode(15, INPUT_PULLUP);    // Fire button
  displayInit();

  if (digitalRead(16) == HIGH) {
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
        updateGameScreen();
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
void handleGameLogic() {
  hitHandle(playerNumber, teamNumber, actualHealth);  
  if (reloading) {
    if (millis() - reloadStartTime >= 3000) {
      actualAmmo = 7;
      reloading = false;
      updateGameScreen();
    }
  } else {
    fireHandle();
  }
}

// Screen methods
void welcomeScreen(){
  clearDisplay();
  writeText(String(teamNumber), 1, 122, 0);
  writeText("PISKOT", 3, 12, 22);
  writeText("START", 1, 98, 56);
}
void updateGameScreen(){
  clearDisplay();
  writeText("Zivoty: " + String(actualHealth), 1, 0, 0);
  writeText(String(teamNumber), 1, 122, 0);
  if(reloading){
    writeText("Nabijam...", 1, 0, 16);
  } else {
    writeText("Naboje: " + String(actualAmmo), 1, 0, 16);
  }
}
void gameOverScreen(){
  clearDisplay();
  writeText("KONIEC", 3, 12, 22);
  writeText("ZNOVA", 1, 98, 56);
}

// Game logic methods
void gameVariablesInit(){
  lastFireBtnState = digitalRead(15);
  choosedWeaponDamage = 1;
  actualHealth = 5;
  actualAmmo = 7;
}
void fireHandle(){
  if(fireBtnPressed()){
    if(actualAmmo > 0){
      sendIRData(playerNumber, teamNumber, choosedWeaponDamage);
      actualAmmo --;
      updateGameScreen();
    }
    else{
      reloadGun();
      // FEAT: AUDIO - no ammo handler (sound of empty gun here)
    }
  }
}
void reloadGun(){
  if (!reloading) {
    reloading = true;
    reloadStartTime = millis();
    updateGameScreen();
  }
};
void decreaseHealth(int damage){
  int newHealth = actualHealth - damage;
  if(newHealth <= 0){
    actualHealth = 0;
  } else{
    actualHealth = newHealth;
  }
  updateGameScreen();
}

// Helper methods
bool fireBtnPressed(){
  bool fireBtnState = digitalRead(15);
  if (fireBtnState == LOW && lastFireBtnState == HIGH) {
    lastFireBtnState = fireBtnState;
    delay(100); // Anti-bounce delay
    return true;
  }
  lastFireBtnState = fireBtnState;
  delay(100); // Anti-bounce delay
  return false;
}
