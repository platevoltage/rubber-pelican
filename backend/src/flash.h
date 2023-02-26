#ifndef FLASH_H
#define FLASH_H

#include "flashdisk.h"
#include "led.h"
#include "keyboard.h"


extern RTC_DATA_ATTR bool flashActivated;
void initializeFlash();
void mountSystemDrive();

#endif
