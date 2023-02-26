#include "config.h"
RTC_DATA_ATTR int blockStartStorage[10];
RTC_DATA_ATTR int nestedWhileStorage;
RTC_DATA_ATTR char varNamesStorage[10][30];
RTC_DATA_ATTR int varValuesStorage[10];
RTC_DATA_ATTR int varCountStorage = 0;
RTC_DATA_ATTR char heldKeysStorage[6][10];
RTC_DATA_ATTR uint32_t ledColor = 0;

void saveStateAndRestart(int varCount, int nestedWhile, int blockStart[], DuckyVariable var[], String heldKeys[], int startIndex) {
      varCountStorage = varCount;
      nestedWhileStorage = nestedWhile;
      for (int i = 0; i < 10; i++) {
        blockStartStorage[i] = blockStart[i];
      }
      for (int i = 0; i < 10; i++) {
        strcpy(varNamesStorage[i], var[i].variableName.c_str());
        varValuesStorage[i] = var[i].value;
      }
      for (int i = 0; i < 6; i++) {
        strcpy(heldKeysStorage[i], heldKeys[i].c_str());
      }
      startOnLineBoot = startIndex+1;
      esp_sleep_enable_timer_wakeup(5 * 100000); // 10 seconds
      esp_deep_sleep_start();
}


void keyboardCallback(String string, int jitterMax) {  
  Serial1.print("PRINTING TO KEYBOARD - ");
  Serial1.println(string);
  // if (!jitter) keyboard.sendString(string);
  // else {
    for (int i = 0; i < string.length(); i++) {
      keyboard.sendChar(string[i]);
      int randomNum = random(1, jitterMax);
      Serial1.println(randomNum);
      vTaskDelay(pdMS_TO_TICKS(randomNum));
    }
  // }
}

void keyboardKeyPressCallback(String key) {  
  Serial1.print("KEY ---");
  Serial1.println(key);
  keyboard.sendKey(getKeyCode(key));

}
void keyboardKeyHoldCallback(String keys[6]) {  
  //also responsible for releasing keys. 
  uint8_t keycodes[6] = {0};
  for (int i = 0; i < 6; i++) {
    if (keyExists(keys[i])) {
      keycodes[i] = getKeyCode(keys[i]);
    } else if (keys[i].length() == 1) {
      uint8_t keycode = (uint8_t) keys[i][0];
      keycodes[i] = keymap[keycode].usage;
    }
  }
  keyboard.sendMultiplePresses(keycodes);
  vTaskDelay(pdMS_TO_TICKS(1));

}

void keyboardShortcutCallback(String key, String modifiers) { 
  keyboard.sendKey(getKeyCode(key), getModCode(modifiers));
}


void delayCallback(int milliseconds) {
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

void ledCallback(uint32_t color) {
    changeLEDColor(color);
}

bool buttonCallback() {
  //after WAIT_FOR_BUTTON_PRESS, the DuckyScript program gets stuck in an infinite loop until this fuction returns TRUE;

  bool buttonPressed = !digitalRead(13);
  bool resume = false;
  vTaskDelay(20 / portTICK_PERIOD_MS);        //this delay acts as a debounce, use delay(20) if you wish
  if (!buttonPressed) {                       //prevents button listening while button already pressed
    while (!resume) {
      buttonPressed = !digitalRead(13);       //listen for button
      resume = buttonPressed; 
      vTaskDelay(1 / portTICK_PERIOD_MS);     //add a short delay to yield to other processes
    }
  }
  return resume;                              //returns false until button is pressed

}

bool enableHIDCallback() {
  bool willRestart = !keyboardActivated;
  keyboardActivated = true;
  return willRestart;
}

bool enableFlashCallback() {
  bool willRestart = !flashActivated;
  flashActivated = true;
  return willRestart;
}

bool disableUSBCallback() {
  bool willRestart = keyboardActivated || flashActivated;
  keyboardActivated = false;
  flashActivated = false;
  return willRestart;
}

bool setUSBPropertiesCallback(int property, String value) {
  bool willRestart = false;
  char* hexStringEnd;
  uint16_t hex;
  switch (property) {
    case _VID: {
      hex = strtol(value.c_str(), &hexStringEnd, 16);
      willRestart = ((VID != hex) && keyboardActivated);
      VID = hex;
      break;
    }
    case _PID: {
      hex = strtol(value.c_str(), &hexStringEnd, 16);
      willRestart = ((PID != hex) && keyboardActivated);
      PID = hex;
      break;
    }
    case _MAN: {
      for (int i = 0; i < value.length(); i++) {
        if (manufacturer[i] != value[i]) {
          willRestart = keyboardActivated;
          manufacturer[i] = value[i];
        } 
      }
      break;
    }
    case _PROD: {
      for (int i = 0; i < value.length(); i++) {
        if (product[i] != value[i]) {
          willRestart = keyboardActivated;
          product[i] = value[i];
        } 
      }
      break;
    }
    case _SERIAL: {
      for (int i = 0; i < value.length(); i++) {
        if (serial[i] != value[i]) {
          willRestart = keyboardActivated;
          serial[i] = value[i];
        } 
      }
      break;
    }
  }
  return willRestart;
}

void restartCallback(int varCount, int nestedWhile, int blockStart[], DuckyVariable var[], String heldKeys[], int startIndex) {
  saveStateAndRestart(varCount, nestedWhile, blockStart, var, heldKeys, startIndex);
}