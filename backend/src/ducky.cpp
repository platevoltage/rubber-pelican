
#include "ducky.h"



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
String * splitModifiers(String string, int * size) {
  string+="-";
  int _size = 0;
  String * modifiers = new String[5];
  while (string.length() > 0) {                                   
    modifiers[_size] = string.substring(0, string.indexOf('-'));  
    string = string.substring(string.indexOf('-') + 1);  
    _size++;
  }
  *size = _size;
  return modifiers;
}

DuckyCommand * splitByLine(String string, int * size) {
  int _size = 0;
  DuckyCommand * commands = new DuckyCommand[100];
  while (string.length() > 0) {   
    String line;                                    // loop until the input string is empty
    line = string.substring(0, string.indexOf('\n'));     // extract the first line and store it in the array
    line.trim();
    commands[_size].instruction = line.substring(0, string.indexOf(' '));  
    if (string.indexOf(' ') >= 0) {
      commands[_size].parameter = line.substring(string.indexOf(' ')+1);  
    } 
    string = string.substring(string.indexOf('\n') + 1);              // remove the first line from the input string                                
    if (line.length() > 0) {
      _size++;    // increment the index of the lines array unless the line is blank  
    }                   
  }
  *size = _size;
  return commands;
}

void duckyBlock(DuckyCommand commands[], size_t commands_t, DuckyCallbacks callbacks) {
  bool inBlock = false;
  int blockStart[10];
  int commandBuffer_t = 0;
  int nestedWhile = 0;
  // int nestedIf = 0;
  bool condition;
  bool execute = true;
  DuckyCommand commandBuffer[100];


  DuckyVariable var[10];
  int varCount = 0;

  int i = 0;
  while( i < commands_t ) {
    // if (nestedWhile == 0) {
      if (commands[i].instruction.equals("STRING") && execute) {
        String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
        parameter = replaceVariables(parameter, var, varCount);
        callbacks.keyboard(parameter);
      }
      else if (commands[i].instruction.equals("STRINGLN") && execute) {
        String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
        parameter = replaceVariables(parameter, var, varCount);
        callbacks.keyboard(parameter + '\n');
      }
      else if (commands[i].instruction.equals("DELAY") && execute) {
        String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
        parameter = replaceVariables(parameter, var, varCount);
        Serial1.println(parameter);
        callbacks.delay(parameter.toInt());

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
        // nestedIf++;
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
        // nestedIf--;
        execute = true;
      }
      else if (
        (
          commands[i].instruction.startsWith("COMMAND") ||
          commands[i].instruction.startsWith("CTRL") ||
          commands[i].instruction.startsWith("ALT") ||
          commands[i].instruction.startsWith("OPTION") ||
          commands[i].instruction.startsWith("SHIFT")
        ) && execute
      ) {
        // String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
        // parameter = replaceVariables(parameter, var, varCount);
        int modifierValue = 0;
        char keycode = commands[i].parameter[0];
        int size = 0;
        String * modifiers = splitModifiers(commands[i].instruction, &size);
        callbacks.keyboardShortcut(modifiers, size, keycode);
      }
    i++;
  }
    delete[] commands;
}


