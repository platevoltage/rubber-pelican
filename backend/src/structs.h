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
    void (*keyboard)(String, bool);
    void (*keyboardKeyPress)(String);
    void (*keyboardKeyHold)(String[]);
    void (*keyboardShortcut)(String, String);
    void (*delay)(int);
    void (*ledColor)(uint32_t);
    bool (*button)();
    bool (*enableHID)();
    bool (*enableFlash)();
    bool (*disableUSB)();
    bool (*setUSBProperties)(int,  String);
    void (*restart)(int, int, int[], DuckyVariable[], String[], int);
} DuckyCallbacks;

#endif