#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "ducky.h"
#include "led.h"
#include "keyboard.h"
#include "structs.h"
#include "hidkeylayout.h"
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#include <esp_system.h>

extern RTC_DATA_ATTR int blockStartStorage[10];
extern RTC_DATA_ATTR int nestedWhileStorage;
extern RTC_DATA_ATTR char varNamesStorage[10][30];
extern RTC_DATA_ATTR int varValuesStorage[10];
extern RTC_DATA_ATTR int varCountStorage;
extern RTC_DATA_ATTR char heldKeysStorage[6][10];
extern RTC_DATA_ATTR uint32_t ledColor;


void keyboardCallback(String string, int jitterMax);
void keyboardKeyPressCallback(String key);
void keyboardKeyHoldCallback(String keys[6]);
void keyboardShortcutCallback(String key, String modifiers);
void delayCallback(int milliseconds);
void ledCallback(uint32_t color);
bool buttonCallback();
bool enableHIDCallback();
bool enableFlashCallback();
bool disableUSBCallback();
bool setUSBPropertiesCallback(int property, String value);
void restartCallback(int varCount, int nestedWhile, int blockStart[], DuckyVariable var[], String heldKeys[], int startIndex);

#endif
