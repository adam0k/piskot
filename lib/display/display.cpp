#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "graphics.h"
#include "weapon.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C   // Display address
#define OLED_RESET -1         // Reset pin (-1 if there is no reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displayInit(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Display allocation failed"));
    while (true); // Stop program, when inicialization failed
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.display();
}

void clearDisplay(String partName = ""){
  if(partName == "LIVES") {
    display.fillRect(0, 0, 90, 16, BLACK);
    display.display();
  } else if(partName == "AMMO"){
    display.fillRect(0, 16, 112, 48, BLACK);
    display.display();
  } else{
    display.clearDisplay();
  }
}

void writeText(String text, int fontSize, int x, int y){
  display.setTextSize(fontSize);  // fontSize 1 = 6x8
  display.setCursor(x, y);
  display.println(text);
  display.display();
}
void drawMakcen(int x, int y){
  display.drawBitmap(x, y, makcenBitmap, 8, 5, WHITE);
  display.display();
}

void drawLives(int lives){
  clearDisplay("LIVES");
  for (int i = 0; i < lives; i++)
  {
    display.drawBitmap(1 + (i*18), 1, heartBitmap, 16, 14, WHITE);
  }
  display.display();
}
void drawAmmo(const Weapon &actualWeapon){
  clearDisplay("AMMO");
  for (int i = 0; i < actualWeapon.currentAmmo; i++)
  {
    display.drawBitmap(0 + (i*15), 31, pistolAmmoBitmap, 16, 32, WHITE);
  }
  display.display();
}
void drawWeaponInfo(const Weapon &actualWeapon){
  writeText((String(actualWeapon.damage) + "x"), 1, 116, 45);
  writeText((String(actualWeapon.reloadTime/1000) + "s"), 1, 116, 56);
}
void drawReloadingAnimation(unsigned long reloadStartTime, unsigned long reloadTime){
  int progress = (millis() - reloadStartTime) * 100 / reloadTime;
  if (progress >= 100) progress = 100;
  display.drawRoundRect(3, 40, 100, 20, 2, WHITE);
  display.fillRect(3, 41, progress, 18, WHITE);

  display.display();
}