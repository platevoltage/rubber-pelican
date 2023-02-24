#ifndef DUCKY_H
#define DUCKY_H

#include <Arduino.h>
#include <tinyexpr.h>
#include "config.h"
#include "structs.h"
#include "flash.h"
#include "_littlefs.h"


// typedef struct {
//   String variableName;
//   int value;
// } DuckyVariable;

// typedef struct {
//   String instruction;
//   String parameter;
// } DuckyCommand;
extern RTC_DATA_ATTR int startOnLineBoot;
double eval(String equation);

bool compare(String equation);

String replaceVariables(String string, DuckyVariable * var, int varCount);

String * splitModifiers(String string, int * size);

DuckyCommand * splitByLine(String string, int * size);

void duckyBlock(DuckyCommand commands[], size_t commands_t, DuckyCallbacks callbacks, int startOnBlock);

#endif



