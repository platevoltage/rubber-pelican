#include <Arduino.h>
#include <ArduinoJson.h>
#include "hidkeyboard.h"
#include "server.hpp"


#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1

HIDkeyboard keyboard;

typedef struct {
  String variableName;
  int value;
} DuckyVariable;

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
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
  String string = server.arg(0);
  Serial1.println(string);
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, string);
  size_t size = doc.size();

  DuckyVariable var[10];
  int varCount = 0;



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
    else if (doc[i].containsKey("VAR")) {
      String codeLine = doc[i]["VAR"];
      var[varCount].variableName = getValue(codeLine, '=', 0);
      var[varCount].variableName.trim();
      var[varCount].value = getValue(codeLine, '=', 1).toInt();

      Serial1.println( var[varCount].variableName );
      Serial1.println( var[varCount].value );
      varCount++;
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


