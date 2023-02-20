#include <Arduino.h>
#include <ArduinoJson.h>
#include <tinyexpr.h>
#include "hidkeyboard.h"
#include "server.hpp"
#include "ducky.hpp"


#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1

HIDkeyboard keyboard;

void keyboardCallBack(String string) {  //duckyBlock uses this function to to decided what happens when a keyboard action is called

  keyboard.sendString(string);

}


void typeString() {
    sendHeaders();
    String string = server.arg(0);
    // Serial1.println(string);
    // keyboard.sendString(string);
    server.send(200, "text/plain", string.c_str());

}

void interpretDuckyScript() {
  sendHeaders();
  Serial1.println(server.arg("plain") + '\n');
  int commands_t = 0;
  DuckyCommand * commands = splitByLine(server.arg("plain") + '\n', &commands_t);
  for (int i = 0; i < commands_t; i++) {
    Serial1.print("Line ");
    Serial1.print(i + 1);
    Serial1.print(": ");
    Serial1.print(commands[i].instruction);
    Serial1.print(" () ");
    Serial1.println(commands[i].parameter);
  }
  duckyBlock(commands, commands_t, keyboardCallBack);
  server.send(200, "text/plain", server.arg("plain").c_str());
}

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  keyboard.begin();
  serverStart(typeString, interpretDuckyScript);
  // interpretDuckyScript();
}

void loop() {
  // put your main code here, to run repeatedly:
  webClientTimer(0);
  yield();
}
