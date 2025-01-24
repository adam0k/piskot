#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

void writeText(String text, int fontSize, int x, int y){
  display.setTextSize(fontSize);  // fontSize 1 = 6x8
  display.setCursor(x, y);
  display.println(text);
  display.display();
}

void clearDisplay(){
  display.clearDisplay();
}