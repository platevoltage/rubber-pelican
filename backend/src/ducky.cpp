
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
  return string;
}
void updateVariable(String varToBeChanged, int newValue,  DuckyVariable * var, int varCount) {
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

DuckyCommand * splitByLine(String string, int * size) {
  int _size = 0;
  DuckyCommand * commands = new DuckyCommand[100];
  while (string.length() > 0) {   
    String line;                                    // loop until the input string is empty
    line = string.substring(0, string.indexOf('\n'));     // extract the first line and store it in the array
    line.trim();
    commands[_size].instruction = line.substring(0, string.indexOf(' '));  
    commands[_size].parameter = line.substring(string.indexOf(' ')+1);  
    string = string.substring(string.indexOf('\n') + 1);              // remove the first line from the input string                                
    if (line.length() > 0) {
      _size++;    // increment the index of the lines array unless the line is blank  
    }                   
  }
  *size = _size;
  return commands;
}

void duckyBlock(DuckyCommand commands[], size_t commands_t, DuckyCallbacks callbacks, DuckyVariable *globalVars, size_t globalVars_t) {
  bool inBlock = false;
  int blockStart = 0;
  int commandBuffer_t = 0;
  String condition;
  DuckyCommand commandBuffer[100];


  DuckyVariable var[10];
  int varCount = 0;

  int i = 0;
  while( i < commands_t ) {
    if (!inBlock) {
      if (commands[i].instruction.equals("STRING")) {
        commands[i].parameter = replaceVariables(commands[i].parameter, var, varCount);
        commands[i].parameter = replaceVariables(commands[i].parameter, globalVars, globalVars_t);
        Serial1.println(commands[i].parameter);
        callbacks.keyboard(commands[i].parameter);
      }
      else if (commands[i].instruction.equals("STRINGLN")) {
        commands[i].parameter = replaceVariables(commands[i].parameter, var, varCount);
        commands[i].parameter = replaceVariables(commands[i].parameter, globalVars, globalVars_t);
        Serial1.println(commands[i].parameter);
        callbacks.keyboard(commands[i].parameter + '\n');
      }
      else if (commands[i].instruction.equals("DELAY")) {
        commands[i].parameter = replaceVariables(commands[i].parameter, var, varCount);
        commands[i].parameter = replaceVariables(commands[i].parameter, globalVars, globalVars_t);
        Serial1.println(commands[i].parameter);
        callbacks.delay(commands[i].parameter.toInt());
        // delay(5000);

      }
      else if (commands[i].instruction.equals("LED_R")) {
        callbacks.ledColor(0xFF0000);
      }
      else if (commands[i].instruction.equals("LED_G")) {
        callbacks.ledColor(0x00FF00);
      }
      else if (commands[i].instruction.equals("LED_B")) {
        callbacks.ledColor(0x0000FF);
      }
      else if (commands[i].instruction.equals("LED_OFF")) {
        callbacks.ledColor(0x000000);
      }
      else if (commands[i].instruction[0] == ('$')) {
        commands[i].parameter = replaceVariables(commands[i].parameter, var, varCount);
        commands[i].parameter = replaceVariables(commands[i].parameter, globalVars, globalVars_t);
        int newValue = eval( commands[i].parameter.substring(commands[i].parameter.indexOf('=')+1) );
        // int newValue = commands[i].parameter.substring(commands[i].parameter.indexOf('=')+1).toInt();
        updateVariable(commands[i].instruction, newValue, var, varCount);
      }
      else if (commands[i].instruction.equals("VAR")) {
        bool varAlreadyDeclared = false;
        for (int j = 0; j < varCount; j++) {
          if (var[j].variableName == commands[i].instruction) {
            var[j].value =  eval( commands[i].parameter.substring(commands[i].parameter.indexOf('=')+1) );
            varAlreadyDeclared = true;
            break;
          }
        }
        if (!varAlreadyDeclared) {
          var[varCount].variableName = commands[i].parameter.substring(0, commands[i].parameter.indexOf('='));  
          var[varCount].variableName.trim();
          var[varCount].value = commands[i].parameter.substring(commands[i].parameter.indexOf('=')+1).toInt();  

          Serial1.println( var[varCount].variableName );
          Serial1.println( var[varCount].value );
          varCount++;
        }
      }
      else if (commands[i].instruction.equals("WHILE")) {
        commands[i].parameter = replaceVariables(commands[i].parameter, var, varCount);
        commands[i].parameter = replaceVariables(commands[i].parameter, globalVars, globalVars_t);
        Serial1.println( compare(commands[i].parameter) );
        inBlock = true;
        blockStart = i;
        Serial1.print("BLOCK START - ");
        Serial1.println(blockStart);
        condition = commands[i].parameter;
      }
    }
    else if (commands[i].instruction.equals("ENDWHILE")) {
      inBlock = false;
      if ( compare( condition ) ) {
        Serial1.print("JUMPING BACK TO - ");
        Serial1.println(blockStart);
        i = blockStart-1;
        for (int k = 0; k < commandBuffer_t; k++) {
          Serial1.println(commandBuffer[k].instruction);
        }
        Serial1.print("FREE-HEAP - ");
        Serial1.println(ESP.getFreeHeap());
        duckyBlock(commandBuffer, commandBuffer_t, callbacks, var, varCount);
      } else {
        commandBuffer_t = 0;
        delete[] commands;
      }
    }
    else {
      commandBuffer[commandBuffer_t] = commands[i];
      commandBuffer_t++;
    }
    i++;
  }
  
}


