#ifndef WEAPON_H
#define WEAPON_H

#include <Arduino.h>

struct Weapon {
  String name;
  int maxAmmo;
  int currentAmmo;
  int damage;
  unsigned long reloadTime;
};

#endif