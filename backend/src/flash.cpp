#include "flash.h"
#include "keyboard.h"

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
    if (dev.init("/fat0", "ffat0")) {
        // if (!flashActivated) dev.setBaseEP(3); // This is a hack that sets the ID to clash with keyboard when deactivated. Simply not initializing the flash causes the keyboard to not work.
        // if (flashActivated || keyboardActivated) {
        if (dev.begin()) {
            Serial1.println("MSC lun 0 begin");
        }
        else Serial1.println("LUN 0 failed");
        }
    
    // }
}
