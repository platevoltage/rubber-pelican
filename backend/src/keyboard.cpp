#include "keyboard.h"

HIDkeyboard keyboard;

void keyboardCallBack(String string) {  
  keyboard.sendString(string);
}