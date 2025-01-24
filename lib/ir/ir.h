#ifndef IR_H
#define IR_H

typedef void (*HitCallback)(int damage);

void irInit(HitCallback callback);
void sendIRData(int myPlayer, int myTeam, int myWeaponDamage);
void hitHandle(int myPlayer, int myTeam, int &actualHealth);
void disableIr();
void enableIr();

#endif