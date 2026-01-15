#include <stdbool.h>

#define KEYBOARD_H
#ifndef KEYBOARD_H

void keyboardDriver();
void sendLightStatusByte();
void setCapsLockLightStatus(bool val);
void setScrollLockLightStatus(bool val);
void setNumLockLightStatus(bool val);

#endif