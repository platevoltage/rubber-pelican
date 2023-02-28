#include "flash.h"

bool RTC_DATA_ATTR flashActivated = false;
bool RTC_DATA_ATTR systemDriveActivated = false;

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

void mountSystemDrive(bool stall) {

    if (dev.init("/fat1", "ffat")) {
        if (dev.begin()) {
            Serial1.println("MSC lun 1 begin");
        }
        else Serial1.println("LUN 1 failed");
    }
    if (dev0.init("/fat0", "ffat0")) {
        if (dev0.begin()) {
            Serial1.println("MSC lun 0 begin");
        }
        else Serial1.println("LUN 0 failed");
    }
    while (stall) { // wait for button push then restart
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
        }
        systemDriveActivated = false;
        flashActivated = false;
        esp_sleep_enable_timer_wakeup(5 * 100000); //restart
        esp_deep_sleep_start();
      }
    }
    
    // }
}

void deleteFile(const char * path) {
    Serial.printf("Deleting file: %s\r\n", path);
    if(FFat.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

void writeFile(const char * path, const char * message) {
  Serial1.printf("Writing file: %s\n", path);
  File file = FFat.open(path, "w");
  if (!file) {
    Serial1.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial1.println("File written");
  } else {
    Serial1.println("Write failed");
  }
  delay(200); // Make sure the CREATE and LASTWRITE times are different
  file.close();
}


String readFile(const char * path) {
  Serial1.printf("Reading file: %s\n", path);
  File file = FFat.open(path, "r");
  if (!file) {
    Serial1.println("Failed to open file for reading");
    return "";
  }
  Serial1.print("Read from file: ");
  String data;

  while (file.available()) {
    data += file.readString();
  }
  file.close();
  return data;
}

void initializeSystemFS() {
    if(!FFat.begin(true, "/fat0", 10, "ffat0")){
        Serial1.println("FAT Mount Failed");
        return;
    }
}