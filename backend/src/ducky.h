#ifndef DUCKY_H
#define DUCKY_H

#include <Arduino.h>
#include <tinyexpr.h>


typedef struct {
  String variableName;
  int value;
} DuckyVariable;

typedef struct {
  String instruction;
  String parameter;
} DuckyCommand;

double eval(String equation);

bool compare(String equation);

String replaceVariables(String string, DuckyVariable var[10], int varCount);

DuckyCommand * splitByLine(String string, int * size);

void duckyBlock(DuckyCommand commands[], size_t commands_t, void (*printToKeyboard)(String));

#endif



