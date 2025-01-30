#ifndef DISPLAY_H
#define DISPLAY_H

#include "weapon.h"

void displayInit();
void clearDisplay(String partName = "");
void writeText(String text, int fontSize, int x, int y);
void drawMakcen(int x, int y);

void drawLives(int lives);
void drawAmmo(const Weapon &actualWeapon);
void drawWeaponInfo(const Weapon &actualWeapon);
void drawReloadingAnimation(unsigned long reloadStartTime, unsigned long reloadTime);

#endif