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
    void (*keyboardShortcut)(String*, int, String);
    void (*delay)(int);
    void (*ledColor)(uint32_t);
} DuckyCallbacks;

#endif