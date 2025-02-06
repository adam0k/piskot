#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

void soundInit();
void playSound(int folder, int file);

#endif