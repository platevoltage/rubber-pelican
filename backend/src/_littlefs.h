#ifndef _LITTLEFS_H
#define _LITTLEFS_H

#include <Arduino.h>
// #include <LittleFS.h>
#include <FFat.h>
#include "FS.h"

void writeFile(const char * path, const char * message);
String readFile(const char * path);
void initializeLittleFS();


#endif