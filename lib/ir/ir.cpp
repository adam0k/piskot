#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "ir.h"

#define IR_LED_PIN 13
#define IR_RECV_PIN 0

IRsend irsend(IR_LED_PIN);
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

HitCallback hitCallback = nullptr;

void irInit(HitCallback callback){
  irsend.begin();
  irrecv.enableIRIn();
  hitCallback = callback;
}

void sendIRData(int myPlayer, int myTeam, int myWeaponDamage) {
  int data = myPlayer * 100 + myTeam * 10 + myWeaponDamage;
  irsend.sendNEC(data, 32);   // NEC protocol, modulation to 38 kHz, 32-bit signal
}

void hitHandle(int myPlayer, int myTeam, int &actualHealth) {
  if (irrecv.decode(&results)) {
    int receivedData = results.value; // Get received data

    // Ignore data that are not in correct form - PTD (111–999)
    if (receivedData < 100 || receivedData > 999) { 
      return; 
    }

    // Data extraction
    int player = receivedData / 100;
    int team = (receivedData / 10) % 10;
    int damage = receivedData % 10;

    // Ignore self-hit and friendly-fire
    if (player == myPlayer || team == myTeam) {
      return;
    }

    // Handle health decrease (by calling callback funct from main.cpp - decreaseHealth() )
    if (hitCallback) {
      hitCallback(damage);
    }

    irrecv.resume();  // Resume data receiving
  }
}

void disableIr(){
  irrecv.disableIRIn();
}

void enableIr(){
  irrecv.enableIRIn();
}
