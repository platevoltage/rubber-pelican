#include <Arduino.h>
#include "hidkeyboard.h"

// #if CFG_TUD_HID
// #define KEYBOARD_I2C_ADDR     0X5f
// #endif


HIDkeyboard keyboard;

void setup() {
  // put your setup code here, to run once:
  keyboard.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  keyboard.sendChar('a');
  delay(1000);
}


