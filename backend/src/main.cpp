#include <Arduino.h>
#include "hidkeyboard.h"

#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1

HIDkeyboard dev;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);

  dev.begin();
dev.sendString("fsdfsdfs"); // send ASCII char


  

  // initializeKeyboard();


}

void loop() {
  // put your main code here, to run repeatedly:
  // int buttonValue = digitalRead(13);
  //    if (buttonValue == LOW){
  //     // If button pushed, turn LED on
  //     Serial1.println("pushed");
  //   }
}
