#include <Arduino.h>
#include "hidkeyboard.h"
#include "server.hpp"


#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1

HIDkeyboard keyboard;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  keyboard.begin();
  serverStart();
}

void loop() {
  // put your main code here, to run repeatedly:
  webClientTimer(0);
  yield() ;
  // keyboard.sendChar('a');
  // delay(1000);a
}


