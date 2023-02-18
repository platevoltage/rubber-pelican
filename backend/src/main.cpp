#include <Arduino.h>
#include <ArduinoJson.h>
#include "hidkeyboard.h"
#include "server.hpp"


#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1

HIDkeyboard keyboard;


void typeString() {
    sendHeaders();
    String string = server.arg(0);
    // Serial1.println(string);
    // keyboard.sendString(string);
    server.send(200, "text/plain", string.c_str());

}

void interpretDuckyScript() {
  sendHeaders();
  String string = server.arg(0);
  Serial1.println(string);
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, string);
  size_t size = doc.size();

  for (int i = 0; i < size; i++) {
    if (doc[i].containsKey("STRING")) {
      String data = doc[i]["STRING"];
      Serial1.print(data);
      keyboard.sendString(data);
    }
    else if (doc[i].containsKey("STRINGLN")) {
      String data = doc[i]["STRINGLN"];
      Serial1.println(data);
      keyboard.sendString(data);
      keyboard.sendChar('\n');
    }
    else if (doc[i].containsKey("DELAY")) {
      int data = doc[i]["DELAY"];
      Serial1.println(data);
      delay(data);

    }


  }

  server.send(200, "text/plain", string.c_str());

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
  yield() ;
  // keyboard.sendChar('a');
  // delay(1000);
  // interpretDuckyScript(F("[{\"STRING\":\"test string no return    \"},{\"STRINGLN\":\"test string with linebreak\"}]"));
}


