#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "led.h"

typedef struct {
    void (*keyboard)(String);
    void (*delay)(int);
    void (*ledColor)(uint32_t);
} DuckyCallbacks;

void keyboardCallback(String string);
void delayCallback(int milliseconds);
void ledCallback(uint32_t color);


#endif
