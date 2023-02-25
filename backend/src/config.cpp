#include "config.h"
RTC_DATA_ATTR int blockStartStorage[10];
RTC_DATA_ATTR int nestedWhileStorage;
RTC_DATA_ATTR char varNamesStorage[10][30];
RTC_DATA_ATTR int varValuesStorage[10];
RTC_DATA_ATTR int varCountStorage = 0;
void saveStateAndRestart(int varCount, int nestedWhile, int blockStart[], DuckyVariable var[], int i) {
      varCountStorage = varCount;
      nestedWhileStorage = nestedWhile;
      for (int i = 0; i < 10; i++) {
        blockStartStorage[i] = blockStart[i];
      }
      for (int i = 0; i < varCount; i++) {
        strcpy(varNamesStorage[i], var[i].variableName.c_str());
        varValuesStorage[i] = var[i].value;
      }
      startOnLineBoot = i+1;
      esp_sleep_enable_timer_wakeup(5 * 100000); // 10 seconds
      esp_deep_sleep_start();
}


void keyboardCallback(String string) {  
  Serial1.print("PRINTING TO KEYBOARD - ");
  Serial1.println(string);
  keyboard.sendString(string);
}

void keyboardKeyPressCallback(String key) {  
  Serial1.print("KEY ---");
  Serial1.println(key);
  keyboard.sendKey(getKeyCode(key));
}

void keyboardShortcutCallback(String key, String modifiers) { 
  Serial1.print("KEY ---");
  Serial1.println(key);
  Serial1.print("MOD ---");
  Serial1.println(modifiers);
  keyboard.sendKey(getKeyCode(key), getModCode(modifiers));
}


void delayCallback(int milliseconds) {
    Serial1.print("DELAY - ");
    Serial1.println(milliseconds);
    // delay(milliseconds);
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

void ledCallback(uint32_t color) {
    Serial1.print("LED COLOR - ");
    Serial1.println(color);
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

void enableHIDCallback(int varCount, int nestedWhile, int blockStart[], DuckyVariable var[], int i) {
    keyboardActivated = true;
    saveStateAndRestart(varCount, nestedWhile, blockStart, var, i);
}

void enableFlashCallback(int varCount, int nestedWhile, int blockStart[], DuckyVariable var[], int i) {
    flashActivated = true;
    saveStateAndRestart(varCount, nestedWhile, blockStart, var, i);
}

void disableUSBCallback(int varCount, int nestedWhile, int blockStart[], DuckyVariable var[], int i) {
    keyboardActivated = false;
    flashActivated = false;
    saveStateAndRestart(varCount, nestedWhile, blockStart, var, i);
}