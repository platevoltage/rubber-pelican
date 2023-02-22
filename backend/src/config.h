#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "ducky.h"
#include "led.h"
#include "keyboard.h"
#include "structs.h"
#include "hidkeylayout.h"



// typedef struct {
//     void (*keyboard)(String);
//     void (*keyboardShortcut)(DuckyCommand);
//     void (*delay)(int);
//     void (*ledColor)(uint32_t);
// } DuckyCallbacks;

void keyboardCallback(String string);
void keyboardShortcutCallback(String modifiers, String key);
void delayCallback(int milliseconds);
void ledCallback(uint32_t color);


#endif
