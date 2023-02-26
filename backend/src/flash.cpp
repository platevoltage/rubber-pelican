#include "flash.h"

// FlashUSB dev;

// char *l1 = "ffat";
// char *l2 = "ffat1";
bool RTC_DATA_ATTR flashActivated = false;

FlashUSB dev;
FlashUSB dev0;

void initializeFlash() {
    if (dev.init("/fat1", "ffat")) {
        if (!flashActivated) dev.setBaseEP(3); // This is a hack that sets the ID to clash with keyboard when deactivated. Simply not initializing the flash causes the keyboard to not work.
        if (flashActivated || keyboardActivated) {
        if (dev.begin()) {
            Serial1.println("MSC lun 1 begin");
        }
        else Serial1.println("LUN 1 failed");
        } 
    }
}

void mountSystemDrive() {
    // initializeFlash();
    if (dev.init("/fat1", "ffat")) {
        // if (!flashActivated) dev.setBaseEP(3); // This is a hack that sets the ID to clash with keyboard when deactivated. Simply not initializing the flash causes the keyboard to not work.
        // if (flashActivated || keyboardActivated) {
        if (dev.begin()) {
            Serial1.println("MSC lun 1 begin");
        }
        else Serial1.println("LUN 1 failed");
        // }
    
    }
    if (dev0.init("/fat0", "ffat0")) {
        if (dev0.begin()) {
            Serial1.println("MSC lun 0 begin");
        }
        else Serial1.println("LUN 0 failed");
    }
    while (1) { // wait for button push then restart
      changeLEDColor(0xFF00FF);
      delay(20);                                   //this delay acts as a debounce.
      if (digitalRead(13)) {                       //prevents button listening while button already pressed
        uint8_t color = 0;
        while (digitalRead(13)) {
          if (color < 64) changeLEDColor(0xFF00FF);
          else if (color < 128) changeLEDColor(0xFF0000);
          else if (color < 192) changeLEDColor(0xFFFF00);
          else if (color < 256) changeLEDColor(0x00FFFF);
          delay(100);
          color+=10;     
          Serial1.println(color);
        }
        esp_sleep_enable_timer_wakeup(5 * 100000); //restart
        esp_deep_sleep_start();
      }
    }
    
    // }
}
