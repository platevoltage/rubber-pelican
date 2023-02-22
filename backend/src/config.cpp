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

void keyboardShortcutCallback(String * modifiers, int size, String key) { 
  int modifierValue = 0;
  int keyValue = 0;
  bool printingKey = false;

  for (int i = 0; i < size; i++) {
    if (modifiers[i].equals("OPTION")) modifierValue += MOD_OPTION;
    if (modifiers[i].equals("ALT")) modifierValue += MOD_ALT;
    else if (modifiers[i].equals("CTRL")) modifierValue += MOD_CTRL;
    else if (modifiers[i].equals("SHIFT")) modifierValue += MOD_SHIFT;
    else if (modifiers[i].equals("COMMAND")) modifierValue += MOD_COMMAND;
    else if (modifiers[i].equals("WINDOWS")) modifierValue += MOD_COMMAND;
    else if (modifiers[i].equals("GUI")) modifierValue += MOD_COMMAND;
  }
  if (key.equals("OPTION")) keyValue = OPTION;
  else if (key.equals("ALT")) keyValue = ALT;
  else if (key.equals("CTRL")) keyValue = CTRL;
  else if (key.equals("SHIFT")) keyValue = SHIFT;
  else if (key.equals("COMMAND")) keyValue = COMMAND;
  else if (key.equals("WINDOWS")) keyValue = COMMAND;
  else if (key.equals("GUI")) keyValue = COMMAND;
  else if (key.equals("ESCAPE")) keyValue = ESCAPE;
  else if (key.equals("ENTER")) keyValue = ENTER;
  else if (key.equals("SPACE")) keyValue = SPACE;
  else if (key.equals("TAB")) keyValue = TAB;
  else if (key.equals("UPARROW")) keyValue = UPARROW;
  else if (key.equals("UP")) keyValue = UPARROW;
  else if (key.equals("DOWNARROW")) keyValue = DOWNARROW;
  else if (key.equals("DOWN")) keyValue = DOWNARROW;
  else if (key.equals("LEFTARROW")) keyValue = LEFTARROW;
  else if (key.equals("LEFT")) keyValue = LEFTARROW;
  else if (key.equals("RIGHTARROW")) keyValue = RIGHTARROW;
  else if (key.equals("PAGEUP")) keyValue = PAGEUP;
  else if (key.equals("PAGEDOWN")) keyValue = PAGEDOWN;
  else if (key.equals("HOME")) keyValue = HOME;
  else if (key.equals("END")) keyValue = END;
  else if (key.equals("INSERT")) keyValue = INSERT;
  else if (key.equals("DELETE")) keyValue = DELETE;
  else if (key.equals("BACKSPACE")) keyValue = BACKSPACE;
  else if (key.equals("PAUSE")) keyValue = PAUSE;
  else if (key.equals("BREAK")) keyValue = BREAK;
  else if (key.equals("PRINTSCREEN")) keyValue = PRINTSCREEN;
  else if (key.equals("MENU")) keyValue = MENU;
  else if (key.equals("APP")) keyValue = APP;
  else if (key.equals("F1")) keyValue = F1;
  else if (key.equals("F2")) keyValue = F2;
  else if (key.equals("F3")) keyValue = F3;
  else if (key.equals("F4")) keyValue = F4;
  else if (key.equals("F5")) keyValue = F5;
  else if (key.equals("F6")) keyValue = F6;
  else if (key.equals("F7")) keyValue = F7;
  else if (key.equals("F8")) keyValue = F8;
  else if (key.equals("F9")) keyValue = F9;
  else if (key.equals("F10")) keyValue = F10;
  else if (key.equals("F11")) keyValue = F11;
  else if (key.equals("F12")) keyValue = F12;
  else printingKey = true;
  
  if (printingKey) keyboard.sendKey(keymap[ key[0] ].usage, modifierValue);
  else keyboard.sendKey(keyValue, modifierValue); 

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