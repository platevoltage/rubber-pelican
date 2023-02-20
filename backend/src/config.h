#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

typedef struct {
    void (*keyboard)(String);
    void (*delay)(int);
} DuckyCallbacks;

void keyboardCallback(String string);
void delayCallback(int milliseconds);


#endif
