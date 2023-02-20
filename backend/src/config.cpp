#include "config.h"

void keyboardCallback(String string) {  
  // keyboard.sendString(string);
  Serial1.print("PRINTING TO KEYBOARD - ");
  Serial1.println(string);
}

void delayCallback(int milliseconds) {
    Serial1.print("DELAY - ");
    Serial1.println(milliseconds);
    // delay(milliseconds);
    vTaskDelay(pdMS_TO_TICKS(milliseconds));
}

void ledCallback(uint32_t color) {
    Serial1.print("LED COLOR - ");
    Serial1.println(color);
    changeLEDColor(color);

}