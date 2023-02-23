#include <Arduino.h>
#include "server.h"
#include "led.h"
#include "flash.h"


#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  pinMode(13, INPUT_PULLUP);

  initializeKeyboard();
  initializeLED();
  initializeFlash();
  xTaskCreatePinnedToCore(serverTask, "Server Task", 10000, NULL, 1, NULL, 1); //webserver gets it's own task

}

void loop() {
  // put your main code here, to run repeatedly:
  // int buttonValue = digitalRead(13);
  //    if (buttonValue == LOW){
  //     // If button pushed, turn LED on
  //     Serial1.println("pushed");
  //   }
}
