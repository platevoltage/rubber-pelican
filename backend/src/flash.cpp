#include "flash.h"

FlashUSB dev;
FlashUSB dev1;
char *l1 = "ffat";
char *l2 = "ffat1";
bool RTC_DATA_ATTR flashActivated = false;

void initializeFlash() {
    if (dev.init("/fat1", "ffat"))
    {
        if (dev.begin())
        {
            Serial1.println("MSC lun 1 begin");
            flashActivated = true;
        }
        else
            log_e("LUN 1 failed");
    }
}
