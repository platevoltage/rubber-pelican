#include <Arduino.h>
#include <ArduinoJson.h>
#include <tinyexpr.h>
#include "hidkeyboard.h"
#include "server.hpp"


#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1

// HIDkeyboard keyboard;

typedef struct {
  String variableName;
  int value;
} DuckyVariable;

typedef struct {
  String instruction;
  String parameter;
} DuckyCommand;


String getValue(String data, char separator, int index) {
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
double eval(String equation) {
      Serial1.print("> ");
      Serial1.println(equation);
      int error;
      double result = te_interp(equation.c_str(), &error);

      if (error) {
        Serial1.print(" ");
        for (int i = 0; i < error; i++) {
          Serial1.print(" ");
        }
        Serial1.println("↑");
        Serial1.println("I didn't understand this part.");
      } else {
        Serial1.printf(" = %.10g\n", result);
      }
      return result;
}

bool compare(String equation) {
  size_t comparators_t = 6;
  String comparators[] = {">=", "<=", "==", "!=", "<", ">"}; //longer ones must go first
  int comparatorSelected = -1;
  int comparatorIndex = -1;
  equation.trim();
  if (equation[0] == '(') equation.remove(0,1);
  if (equation[equation.length()-1] == ')') equation.remove(equation.length()-1,1);
  equation.trim();
  for (int i=0; i < comparators_t; i++) {
    int _comparatorIndex = equation.indexOf(comparators[i]);
    if (_comparatorIndex > 0) {
      Serial1.println(comparators[i]);
      comparatorSelected = i;
      comparatorIndex = _comparatorIndex;
      break;
    }
  }
  bool result = true;
  if (comparatorIndex > -1) {

    String leftString = equation.substring(0, comparatorIndex);
    String rightString = equation.substring(comparatorIndex+comparators[comparatorSelected].length(), equation.length());
    double left = eval(leftString);
    double right = eval(rightString);
    Serial1.println(left);
    Serial1.println(comparators[comparatorSelected]);
    Serial1.println(right);
    switch (comparatorSelected) {
      case 0: {
        result = (left >= right);
        break;
      }
      case 1: {
        result = (left <= right);
        break;
      }
      case 2: {
        result = (left == right);
        break;
      }
      case 3: {
        result = (left != right);
        break;
      }
      case 4: {
        result = (left < right);
        break;
      }
      case 5: {
        result = (left > right);
        break;
      }
      default: result = false;
    }
  } else {
    Serial.println(equation);
    result = eval(equation);
  }
  return result;
}

String replaceVariables(String string, DuckyVariable var[10], int varCount) {
  for (int i = 0; i < varCount; i++) {
    if (string.indexOf(var[i].variableName) > -1) {
      Serial1.print("replace variable - ");
      Serial1.println(var[i].variableName);
      
      string.replace(var[i].variableName, String(var[i].value) );
    }
  }
  Serial1.print("replace variable result - ");
  Serial1.println(string);
  return string;
}

String * splitByLine(String string, int * size) {
  int _size = 0;
  String * lines = new String[100];
  while (string.length() > 0) {                                       // loop until the input string is empty
    lines[_size] = string.substring(0, string.indexOf('\n'));     // extract the first line and store it in the array
    lines[_size].trim();
    string = string.substring(string.indexOf('\n') + 1);              // remove the first line from the input string                                
    if (lines[_size].length() > 0) _size++;                   // increment the index of the lines array unless the line is blank    
  }
  *size = _size;
  return lines;
}

void interpretDuckyScript() {
  sendHeaders();
  String string = server.arg("plain") + '\n';
  Serial1.println(string);
  int lines_t;
  String * lines = splitByLine(string, &lines_t); ;


  for (int i = 0; i < lines_t; i++) {
    Serial1.print("Line ");
    Serial1.print(i + 1);
    Serial1.print(": ");
    Serial1.println(lines[i]);
  }

  // DynamicJsonDocument doc(1024);
  // deserializeJson(doc, string);
  // size_t size = doc.size();

  // DuckyVariable var[10];
  // int varCount = 0;


  // for (int i = 0; i < size; i++) {
  //   if (doc[i].containsKey("STRING")) {
  //     String data = doc[i]["STRING"];
  //     data = replaceVariables(data, var, varCount);
  //     Serial1.print(data);
  //     keyboard.sendString(data);
  //   }
  //   else if (doc[i].containsKey("STRINGLN")) {
  //     String data = doc[i]["STRINGLN"];
  //     data = replaceVariables(data, var, varCount);
  //     Serial1.println(data);
  //     keyboard.sendString(data);
  //     keyboard.sendChar('\n');
  //   }
  //   else if (doc[i].containsKey("DELAY")) {
  //     String data = doc[i]["DELAY"];
  //     data = replaceVariables(data, var, varCount);
  //     Serial1.println(data);
  //     delay(data.toInt());

  //   }
  //   else if (doc[i].containsKey("VAR")) {
  //     String codeLine = doc[i]["VAR"];
  //     var[varCount].variableName = getValue(codeLine, '=', 0);
  //     var[varCount].variableName.trim();
  //     var[varCount].value = getValue(codeLine, '=', 1).toInt();

  //     Serial1.println( var[varCount].variableName );
  //     Serial1.println( var[varCount].value );
  //     varCount++;
  //   }
  //   else if (doc[i].containsKey("WHILE")) {
  //     String codeLine = doc[i]["WHILE"];
  //     codeLine = replaceVariables(codeLine, var, varCount);
  //     // Serial1.println(codeLine);
  //     Serial1.println( compare(codeLine) );
  //   }
  // }
  delete[] lines;
  server.send(200, "text/plain", string.c_str());

}

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  // keyboard.begin();
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


