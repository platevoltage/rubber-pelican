#include "ducky.h"

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
  size_t numComparators = 6;
  String comparators[] = {">=", "<=", "==", "!=", "<", ">"}; //longer ones must go first
  int comparatorSelected = -1;
  int comparatorIndex = -1;
  equation.trim();
  if (equation[0] == '(') equation.remove(0,1);
  if (equation[equation.length()-1] == ')') equation.remove(equation.length()-1,1);
  equation.trim();
  for (int i=0; i < numComparators; i++) {
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

  //find and replace
  DuckyConstant constants[100];
  int count = 0;
  for (int i = 0; i < _size; i++) {
    if (commands[i].instruction.equals("DEFINE") && commands[i].parameter.length()) {
      String name = commands[i].parameter.substring(0, commands[i].parameter.indexOf(' '));
      String value = commands[i].parameter.substring(commands[i].parameter.indexOf(' ')+1);
      constants[count].variableName = name;
      constants[count].value = value;
      count++;
    }
  }
  for (int i = 0; i < _size; i++) {
    for (int j = 0; j < count; j++) {
      if (commands[i].parameter.indexOf( constants[j].variableName ) > -1) {
        commands[i].parameter.replace(constants[j].variableName, constants[j].value);
      }
    }
  }

  return commands;
}

void duckyBlock(DuckyCommand commands[], size_t numCommands, DuckyCallbacks callbacks, int startOnBlock) {
  int blockStart[10];
  int nestedWhile = 0;
  int functionPlaceholder = -1;
  int functionReturnValue = 0;
  String heldKeys[6] = {""};
  bool execute = true;
  DuckyVariable var[10];
  var[0] = {"$_JITTER_ENABLED", 0};
  var[1] = {"$_JITTER_MAX", 20};
  int varCount = 2; //2 vars already defined above
  int i = startOnBlock;
  if (startOnBlock > 0) {
      changeLEDColor(ledColor);
      varCount = varCountStorage;
      nestedWhile = nestedWhileStorage;
      for (int i = 0; i < 10; i++) {
        blockStart[i] = blockStartStorage[i];
      }
      for (int i = 0; i < varCount; i++) {
        var[i].variableName = varNamesStorage[i];
        var[i].value = varValuesStorage[i];
      }
      for (int i = 0; i < 6; i++) {
        heldKeys[i] = heldKeysStorage[i];

      }
      callbacks.delay(200);
      callbacks.keyboardKeyHold(heldKeys);
  }
  while( i < numCommands ) {
    if (commands[i].instruction.equals("STRING") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
      int jitter = 0;
      for (int i = 0; i < varCount; i++) {
        if (var[i].variableName.equals("$_JITTER_MAX")) jitter = var[i].value;
        if (var[i].variableName.equals("$_JITTER_ENABLED") && var[i].value == 0) {
          jitter = 0;
          break;
        }
      }
      callbacks.keyboard(parameter, jitter);
    }
    else if (commands[i].instruction.equals("STRINGLN") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
      int jitter = 0;
      for (int i = 0; i < varCount; i++) {
        if (var[i].variableName.equals("$_JITTER_MAX")) jitter = var[i].value;
        if (var[i].variableName.equals("$_JITTER_ENABLED") && var[i].value == 0) {
          jitter = 0;
          break;
        }
      }
      callbacks.keyboard(parameter + '\n', jitter);
    }
    else if (commands[i].instruction.equals("DELAY") && execute) {
      String parameter = commands[i].parameter.substring(0, commands[i].parameter.length());
      parameter = replaceVariables(parameter, var, varCount);
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
    }
    else if (commands[i].instruction.equals("ELSE")) {
      execute = !execute;
    }
    else if (commands[i].instruction.equals("END_WHILE")) {
      nestedWhile--;
      if (execute) i = blockStart[nestedWhile]-1;
      else {
        execute = true;
      } 
    }
    else if (commands[i].instruction.equals("END_IF")) {
      execute = true;
    }
    else if (commands[i].instruction.equals("FUNCTION")) {
      execute = false;
    }
    else if (commands[i].instruction.equals("END_FUNCTION")) {
      if (!execute) execute = true;
      else i = functionPlaceholder;
    }
    else if (commands[i].instruction.equals("RETURN") && execute) {
      
      String parameter = commands[i].parameter;
      parameter = replaceVariables(parameter, var, varCount);
      functionReturnValue = eval(parameter);
      Serial1.print("XXX - ");
      Serial1.print(commands[i].instruction);
      Serial1.print(" ");
      Serial1.println(commands[i].parameter);
      Serial1.print("RETURN VALUE - ");
      Serial1.println(functionReturnValue);
      i = functionPlaceholder;
    }
    else if (commands[i].instruction.endsWith("()") && execute) {
      functionPlaceholder = i;
      for (int j = i; j >= 0; j--) { //search for function definition
        if (commands[j].instruction.equals("FUNCTION") && commands[j].parameter == commands[i].instruction) {
          Serial1.print("FOUND FUNCTION ");
          Serial1.print(commands[i].instruction);
          Serial1.print(" AT LINE ");
          Serial1.println(j);
          i = j;
          break;
        }
      }
    }


    else if (commands[i].instruction.equals("HOLD") && execute) {
      String key = commands[i].parameter;
      int openSlot = 0;
      bool alreadyPressed = false;
      for (int i = 0; i < 6; i++) {
        if (heldKeys[i].equals(key)) alreadyPressed = true;
      }
      for (int i = 0; i < 6; i++) {
        if (heldKeys[i].equals("")) break;
        else openSlot++;
      }
      if (!alreadyPressed) {
      heldKeys[openSlot] = key;
      }
      callbacks.keyboardKeyHold(heldKeys);
    }
    else if (commands[i].instruction.equals("RELEASE") && execute) {
        String key = commands[i].parameter;
        for (int i = 0; i < 6; i++) {
          if (heldKeys[i].equals(key)) heldKeys[i] = "";
        }
        callbacks.keyboardKeyHold(heldKeys);
    }
    else if (keyExists(commands[i].instruction) && commands[i].parameter.length() == 0 && execute) {  //handles non printing keys
      callbacks.keyboardKeyPress(commands[i].instruction);
    }
    else if (modExists(commands[i].instruction) && execute) {             //checks for mod keys
      bool inject_mod_enabled = commands[i-1].instruction.equals("INJECT_MOD");
      if ((commands[i].instruction.indexOf("WINDOWS") > -1 || commands[i].instruction.indexOf("GUI") > -1) && !inject_mod_enabled) {
        Serial1.println("WINDOWS or GUI keys used as modifiers requires INJECT_MOD on previous line");
      } else {
        callbacks.keyboardShortcut(commands[i].parameter, commands[i].instruction);
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
    else if (commands[i].instruction.equals("WAIT_FOR_CAPS_ON") && execute) {
      while (!getCapsLockStatus()) {}
    }
    else if (commands[i].instruction.equals("WAIT_FOR_CAPS_OFF") && execute) {
      while (getCapsLockStatus()) {}
    }
    else if (commands[i].instruction.equals("WAIT_FOR_CAPS_CHANGE") && execute) {
      bool state = getCapsLockStatus();
      while (state == getCapsLockStatus()) {}
    }
    else if (commands[i].instruction.equals("WAIT_FOR_NUM_ON") && execute) {
      while (!getNumLockStatus()) {}
    }
    else if (commands[i].instruction.equals("WAIT_FOR_NUM_OFF") && execute) {
      while (getNumLockStatus()) {}
    }
    else if (commands[i].instruction.equals("WAIT_FOR_NUM_CHANGE") && execute) {
      bool state = getNumLockStatus();
      while (state == getNumLockStatus()) {}
    }
    else if (commands[i].instruction.equals("WAIT_FOR_SCROLL_ON") && execute) {
      while (!getScrollLockStatus()) {}
    }
    else if (commands[i].instruction.equals("WAIT_FOR_SCROLL_OFF") && execute) {
      while (getScrollLockStatus()) {}
    }
    else if (commands[i].instruction.equals("WAIT_FOR_SCROLL_CHANGE") && execute) {
      bool state = getScrollLockStatus();
      while (state == getScrollLockStatus()) {}
    }
    else if (commands[i].instruction.equals("ATTACKMODE") && execute) {
      bool willRestart = false;
      if (commands[i].parameter.indexOf("MAN_") > -1) {
        int beginningIndex = commands[i].parameter.indexOf("MAN_")+4;
        String temp = commands[i].parameter.substring(beginningIndex);
        willRestart = callbacks.setUSBProperties(_MAN, temp);
      }
      if (commands[i].parameter.indexOf("PROD_") > -1) {
        int beginningIndex = commands[i].parameter.indexOf("PROD_")+5;
        String temp = commands[i].parameter.substring(beginningIndex);
        temp = temp.substring(0, temp.indexOf(" ")) + '\0';
        willRestart = callbacks.setUSBProperties(_PROD, temp);
      }
      if (commands[i].parameter.indexOf("SERIAL_") > -1) {
        int beginningIndex = commands[i].parameter.indexOf("SERIAL_")+7;
        String temp = commands[i].parameter.substring(beginningIndex);
        temp = temp.substring(0, temp.indexOf(" ")) + '\0';
        if (temp.equals("RANDOM")) {
          long randomNumber = random(10000000, 99999999);
          temp = String(randomNumber) + String(randomNumber/2);
          temp = temp.substring(0, 12);
        } 
        willRestart = callbacks.setUSBProperties(_SERIAL, temp);
      }
      if (commands[i].parameter.indexOf("VID_") > -1) {
        String temp = commands[i].parameter.substring(commands[i].parameter.indexOf("VID_")+4, commands[i].parameter.indexOf("VID_")+8);
        char* hexStringEnd;
        willRestart = callbacks.setUSBProperties(_VID, temp);
      }
      if (commands[i].parameter.indexOf("PID_") > -1) {
        String temp = commands[i].parameter.substring(commands[i].parameter.indexOf("PID_")+4, commands[i].parameter.indexOf("PID_")+8);
        char* hexStringEnd;
        willRestart = callbacks.setUSBProperties(_PID, temp);
      }
      if (commands[i].parameter.indexOf("OFF") > -1) {
          willRestart = callbacks.disableUSB(); 
      }
      if (commands[i].parameter.indexOf("HID") > -1) {
          willRestart = callbacks.enableHID();
      }
      if (commands[i].parameter.indexOf("STORAGE") > -1) {
          willRestart = callbacks.enableFlash();
      }
      if (willRestart) {
        callbacks.restart(varCount, nestedWhile, blockStart, var, heldKeys, i);
      }
    }
    else if (commands[i].instruction.equals("REM")) {}
    else if (commands[i].instruction.equals("DEFINE")) {}
    else {
      Serial1.print("COMMAND NOT FOUND - ");
      Serial1.print(commands[i].instruction);
      Serial1.print(" ");
      Serial1.println(commands[i].parameter);
    }
    Serial1.print("CURRENT LINE - ");
    Serial1.print(i);
    Serial1.print(" - EXECUTE ");
    Serial1.println(execute);

    i++;
  }
    // deleteFile("/.cache.txt");
    delete[] commands;
}


