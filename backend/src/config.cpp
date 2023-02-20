#include "config.h"

void keyboardCallback(String string) {  
  // keyboard.sendString(string);
  Serial1.print("PRINTING TO KEYBOARD - ");
  Serial1.println(string);
}

void delayCallback(int milliseconds) {
    delay(milliseconds);
}