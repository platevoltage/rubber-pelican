#include "flash.h"
#include "keyboard.h"

// FlashUSB dev;

char *l1 = "ffat";
char *l2 = "ffat1";
bool RTC_DATA_ATTR flashActivated = false;

FlashUSB dev;

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
