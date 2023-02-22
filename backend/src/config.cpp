#include "config.h"

void keyboardCallback(String string) {  
  // keyboard.sendString(string);
  Serial1.print("PRINTING TO KEYBOARD - ");
  Serial1.println(string);
}
      // keyboard.sendPress(0, 1); //ctrl
      // keyboard.sendPress(0, 2); //shift
      // keyboard.sendPress(0, 3); //ctrl/shift
      // keyboard.sendPress(0, 4);  //option
      // keyboard.sendPress(0, 5);   //ctrl/option
      // keyboard.sendPress(0, 6); //option/shift
      // keyboard.sendPress(0, 7); //option/shift/ctrl
      // keyboard.sendPress(0, 8); //command
      // keyboard.sendPress(0, 9); //command/ctrl
      // keyboard.sendPress(0, 10); //command/shift
      // keyboard.sendPress(0, 11); //command/shift/ctrl
      // keyboard.sendPress(0, 12); //option/command
      // keyboard.sendPress(0, 13); //option/command/ctrl
      // keyboard.sendPress(0, 14); //command/option/shift
      // keyboard.sendPress(0, 15); //command/shift/ctrl/option

void keyboardShortcutCallback(String modifiers, String key) { 
  // int modifierValue = 0;
  // int keyValue = 0;
  // bool printingKey = false;

  // for (int i = 0; i < size; i++) {
  //   if (modifiers[i].equals("OPTION")) modifierValue += MOD_OPTION;
  //   if (modifiers[i].equals("ALT")) modifierValue += MOD_ALT;
  //   else if (modifiers[i].equals("CTRL")) modifierValue += MOD_CTRL;
  //   else if (modifiers[i].equals("SHIFT")) modifierValue += MOD_SHIFT;
  //   else if (modifiers[i].equals("COMMAND")) modifierValue += MOD_COMMAND;
  //   else if (modifiers[i].equals("WINDOWS")) modifierValue += MOD_COMMAND;
  //   else if (modifiers[i].equals("GUI")) modifierValue += MOD_COMMAND;
  // }

  
  keyboard.sendKey(getKeyCode(key), getModCode(modifiers));


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