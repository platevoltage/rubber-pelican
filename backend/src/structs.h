#ifndef STRUCTS_H
#define STRUCTS_H

#include <Arduino.h>

typedef struct {
  String variableName;
  int value;
} DuckyVariable;

typedef struct {
  String instruction;
  String parameter;
} DuckyCommand;

typedef struct {
    void (*keyboard)(String);
    void (*keyboardKeyPress)(String);
    void (*keyboardShortcut)(String, String);
    void (*delay)(int);
    void (*ledColor)(uint32_t);
    bool (*button)();
    void (*enableHID)(int, int, int[], DuckyVariable[], int);
    void (*enableFlash)(int, int, int[], DuckyVariable[], int);
    void (*disableUSB)(int, int, int[], DuckyVariable[], int);
} DuckyCallbacks;

#endif