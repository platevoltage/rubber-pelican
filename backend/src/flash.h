#ifndef FLASH_H
#define FLASH_H

#include "flashdisk.h"
#include "led.h"
#include "keyboard.h"
#include <FFat.h>
#include "FS.h"


extern RTC_DATA_ATTR bool flashActivated;
void initializeFlash();
void mountSystemDrive();
void writeFile(const char * path, const char * message);
String readFile(const char * path);
void initializeSystemFS();


#endif
