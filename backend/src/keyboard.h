#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "hidkeyboard.h"

// extern HIDkeyboard keyboard;
void initializeKeyboard();
void keyboardCallBack(String string); //duckyBlock uses this function to to decided what happens when a 

#endif
