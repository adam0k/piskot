#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include "display.h"

SoftwareSerial mySoftwareSerial(D7, D8); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void soundInit() {
    mySoftwareSerial.begin(9600);
    if (!myDFPlayer.begin(mySoftwareSerial)) {
        Serial.println("DFPlayer Mini not detected!");
        return;
    }
    myDFPlayer.volume(20);  // 0-30   // FEAT Volume controll(+off)
    myDFPlayer.EQ(DFPLAYER_EQ_BASS);
    Serial.println("DFPlayer Mini initialized!");
}

void playSound(int folder, int file){
  if(myDFPlayer.available()){
    myDFPlayer.playFolder(folder, file);
  } else{
    Serial.println("DFPlayer not available...");
  }
}