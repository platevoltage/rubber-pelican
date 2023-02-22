#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "hidkeyboard.h"
#define COMMAND 8
#define CTRL 1
#define OPTION 4
#define ALT 4
#define SHIFT 2



extern HIDkeyboard keyboard;
void initializeKeyboard();
// void callback(String string); //duckyBlock uses this function to to decided what happens when a 

#endif
