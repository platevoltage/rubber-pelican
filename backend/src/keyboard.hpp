#include "hidkeyboard.h"

HIDkeyboard keyboard;

void keyboardCallBack(String string) {  //duckyBlock uses this function to to decided what happens when a keyboard action is called
  keyboard.sendString(string);
}
