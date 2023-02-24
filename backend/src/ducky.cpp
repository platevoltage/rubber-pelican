
#include "ducky.h"
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>

// RTC_DATA_ATTR int startOnLineBoot = 0;
RTC_DATA_ATTR int startOnLineBoot = 0;
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

String replaceVariables(String string, DuckyVariable * var, int varCount) {
  for (int i = 0; i < varCount; i++) {
    int variableIndex = string.indexOf(var[i].variableName);
    if ( variableIndex > -1 
      && (string[var[i].variableName.length() + variableIndex ] == ' ' 
      || var[i].variableName.length() + variableIndex == string.length() 
    )) {
      string.replace(var[i].variableName, String(var[i].value) );
    }
  }

  if ( string.indexOf("FALSE") > -1 ) { //This might need to be looked at
    string.replace("FALSE", "0");
  }
  if ( string.indexOf("TRUE") > -1 ) {
    string.replace("TRUE", "1");
  }
  return string;
}

void updateVariable(String varToBeChanged, int newValue, DuckyVariable * var, int varCount) {
  for (int i = 0; i < varCount; i++) {
    if ( var[i].variableName == varToBeChanged) {
      var[i].value = newValue;
      Serial1.print("CHANGING VARIABLE ");
      Serial1.print(varToBeChanged);
      Serial1.print(" TO - ");
      Serial1.println(newValue);
      break;
    }
  }
}
// String * splitModifiers(String string, int * size) {
//   string+="-";
//   int _size = 0;
//   String * modifiers = new String[5];
//   while (string.length() > 0) {                                   
//     modifiers[_size] = string.substring(0, string.indexOf('-'));  
//     string = string.substring(string.indexOf('-') + 1);  
//     _size++;
//   }
//   *size = _size;
//   return modifiers;
// }

DuckyCommand * splitByLine(String string, int * size) {
  int _size = 0;
  DuckyCommand * commands = new DuckyCommand[100];
  while (string.length() > 0) {   // loop until the input string is empty
    String line = string.substring(0, string.indexOf('\n'));     // extract the first line and store it in the array
    line.trim();
    commands[_size].instruction = line.substring(0, line.indexOf(' '));  
    if (string.indexOf(' ') >= 0) {
      commands[_size].parameter = line.substring(line.indexOf(' ')+1);  
    } 
    string = string.substring(string.indexOf('\n') + 1);              // remove the first line from the input string                                
    if (line.length() > 0) {
      _size++;    // increment the index of the lines array unless the line is blank  
    }                   
  }
  *size = _size;
  return commands;
}



void duckyBlock(DuckyCommand commands[], size_t commands_t, DuckyCallbacks callbacks, int startOnBlock) {
  bool inBlock = false;
  int blockStart[10];
  int nestedWhile = 0;
  bool execute = true;

  DuckyVariable var[10];
  int varCount = 0;

  int i = 0;
  while( i < commands_t ) {
    if (commands[i].instruction.equals("STRING") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
      if (startOnBlock < i) callbacks.keyboard(parameter);
    }
    else if (commands[i].instruction.equals("STRINGLN") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
      if (startOnBlock < i) callbacks.keyboard(parameter + '\n');
    }
    else if (commands[i].instruction.equals("DELAY") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
      Serial1.println(parameter);
      if (startOnBlock < i) callbacks.delay(parameter.toInt());
    }
    else if (commands[i].instruction.equals("LED_R") && execute) {
      callbacks.ledColor(0xFF0000);
    }
    else if (commands[i].instruction.equals("LED_G") && execute) {
      callbacks.ledColor(0x00FF00);
    }
    else if (commands[i].instruction.equals("LED_B") && execute) {
      callbacks.ledColor(0x0000FF);
    }
    else if (commands[i].instruction.equals("LED_OFF") && execute) {
      callbacks.ledColor(0x000000);
    }
    else if (commands[i].instruction[0] == ('$') && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
      int newValue = eval( parameter.substring(parameter.indexOf('=')+1) );
      updateVariable(commands[i].instruction, newValue, var, varCount);
    }
    else if (commands[i].instruction.equals("VAR") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
      bool varAlreadyDeclared = false;
      for (int j = 0; j < varCount; j++) {
        if (var[j].variableName == commands[i].instruction) {
          var[j].value =  eval( parameter.substring(parameter.indexOf('=')+1) );
          varAlreadyDeclared = true;
          break;
        }
      }
      if (!varAlreadyDeclared) {
        var[varCount].variableName = parameter.substring(0, parameter.indexOf('='));  
        var[varCount].variableName.trim();
        var[varCount].value = parameter.substring(parameter.indexOf('=')+1).toInt();  

        Serial1.println( var[varCount].variableName );
        Serial1.println( var[varCount].value );
        varCount++;
      }
    }
    else if (commands[i].instruction.equals("WHILE") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
      if (compare ( parameter )) {
        execute = true;
      } else {
        execute = false;
      }
      blockStart[nestedWhile] = i;
      nestedWhile++;
    }
    else if (commands[i].instruction.equals("IF") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);

      if (compare ( parameter )) {
        execute = true;
      } else {
        execute = false;
      }
    }
    else if (commands[i].instruction.equals("ELSE")) {
      execute = !execute;
    }
    else if (commands[i].instruction.equals("ENDWHILE")) {
      nestedWhile--;
      if (execute) i = blockStart[nestedWhile]-1;
      else {
        execute = true;
      } 
    }
    else if (commands[i].instruction.equals("ENDIF")) {
      execute = true;
    }
    else if (keyExists(commands[i].instruction) && commands[i].parameter.length() == 0 && execute) {  //handles non printing keys
      if (startOnBlock < i) callbacks.keyboardKeyPress(commands[i].instruction);
    }
    else if (modExists(commands[i].instruction) && execute) {             //checks for mod keys
      bool inject_mod_enabled = commands[i-1].instruction.equals("INJECT_MOD");
      if ((commands[i].instruction.indexOf("WINDOWS") > -1 || commands[i].instruction.indexOf("GUI") > -1) && !inject_mod_enabled) {
        Serial1.println("WINDOWS or GUI keys used as modifiers requires INJECT_MOD on previous line");
      } else {
        if (startOnBlock < i) callbacks.keyboardShortcut(commands[i].parameter, commands[i].instruction);
      }
    }
    else if (commands[i].instruction.equals("WAIT_FOR_BUTTON_PRESS") && execute) {
      Serial1.println("WAITING FOR BUTTON PRESS");
      execute = false;
      while (!execute) {
        if (startOnBlock < i) execute = callbacks.button();
        else execute = true;
      }
    }
    else if (commands[i].instruction.equals("ATTACKMODE") && execute) {
      if (commands[i].parameter.indexOf("OFF") > -1) {
        if (keyboardActivated || flashActivated) {
          Serial1.println("ATTACKMODE OFF XXXXX");
          writeFile("/resume.txt", "1");
          esp_task_wdt_init(1, true); //restart ESP32
          esp_task_wdt_add(NULL);
          while(true);  // wait for watchdog timer to be triggered
        }
      }
      if (commands[i].parameter.indexOf("HID") > -1 && !keyboardActivated) {
        initializeKeyboard();
      }
      if (commands[i].parameter.indexOf("STORAGE") > -1 && !flashActivated) {
        initializeFlash();
          // esp_sleep_enable_timer_wakeup(10 * 1000000); // 10 seconds
          // esp_light_sleep_start();
      }
    }
    else if (commands[i].instruction.equals("RESTART") && execute && (startOnBlock < i)) {
      startOnLineBoot = i+1;
          esp_sleep_enable_timer_wakeup(5 * 100000); // 10 seconds
          esp_deep_sleep_start();
    }
    i++;
  }
    startOnLineBoot = 0;
    delete[] commands;
}


