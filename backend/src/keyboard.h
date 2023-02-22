#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "hidkeyboard.h"
#define MOD_COMMAND 8
#define MOD_CTRL 1
#define MOD_OPTION 4
#define MOD_ALT 4
#define MOD_SHIFT 2

#define COMMAND 0xE3
#define CTRL 0xE0
#define OPTION 0xE2
#define ALT 0xE2
#define SHIFT 0xE1
#define ESCAPE 0x29
#define ENTER 0x28
#define SPACE 0x2c



extern HIDkeyboard keyboard;
void initializeKeyboard();
// void callback(String string); //duckyBlock uses this function to to decided what happens when a 

#endif
