#include "keyboard.h"


HIDkeyboard keyboard;
RTC_DATA_ATTR bool keyboardActivated = false;

DuckyKeyMap duckyModMap[NUM_OF_MODS] = {
    {"COMMAND", MOD_COMMAND},
    {"WINDOWS", MOD_WINDOWS},
    {"GUI", MOD_GUI},
    {"CTRL", MOD_CTRL},
    {"OPTION", MOD_OPTION},
    {"ALT", MOD_ALT},
    {"SHIFT", MOD_SHIFT},
};

DuckyKeyMap duckyKeyMap[NUM_OF_KEYS] = {
    {"COMMAND", COMMAND},
    {"WINDOWS", WINDOWS},
    {"GUI", GUI},
    {"CTRL", CTRL},
    {"OPTION", OPTION},
    {"ALT", ALT},
    {"SHIFT", SHIFT},
    {"ESCAPE", ESCAPE},
    {"ENTER", ENTER},
    {"SPACE", SPACE},
    {"TAB", TAB},
    {"UPARROW", UPARROW},
    {"DOWNARROW", DOWNARROW},
    {"LEFTARROW", LEFTARROW},
    {"RIGHTARROW", RIGHTARROW},
    {"UP", UP},
    {"DOWN", DOWN},
    {"LEFT", LEFT},
    {"RIGHT", RIGHT},
    {"PAGEUP", PAGEUP},
    {"PAGEDOWN", PAGEDOWN},
    {"HOME", HOME},
    {"END", END},
    {"INSERT", INSERT},
    {"DELETE", DELETE},
    {"BACKSPACE", BACKSPACE},
    {"PAUSE", PAUSE},
    {"BREAK", BREAK},
    {"PRINTSCREEN", PRINTSCREEN},
    {"MENU", MENU},
    {"APP", APP},
    {"F1", F1},
    {"F2", F2},
    {"F3", F3},
    {"F4", F4},
    {"F5", F5},
    {"F6", F6},
    {"F7", F7},
    {"F8", F8},
    {"F9", F9},
    {"F10", F10},
    {"F11", F11},
    {"F12", F12},
    {"CAPSLOCK", CAPSLOCK},
    {"NUMLOCK", NUMLOCK},
    {"SCROLLLOCK", SCROLLLOCK},

};
RTC_DATA_ATTR uint16_t VID = 0x05ac;
RTC_DATA_ATTR uint16_t PID = 0x021e;
RTC_DATA_ATTR char manufacturer[15] = "GarrettSoft";
RTC_DATA_ATTR char product[15] = "Rubber-Pelican";
RTC_DATA_ATTR char serial[15] = "";

void initializeKeyboard() {
  keyboard.setBaseEP(3);
  keyboard.deviceID(VID,PID);
  keyboard.manufacturer(manufacturer);
  keyboard.product(product); // product name
  keyboard.serial(serial);  // serial number SN
  keyboard.begin();
  Serial1.println("KEYBOARD STARTED");
  keyboard.sendString("jhfjsdfsd");
  keyboardActivated = true;
}

String * splitModifiers(String string, int * size) {
  string+="-";
  int _size = 0;
  String * modifiers = new String[5];
  while (string.length() > 0) {                                   
    modifiers[_size] = string.substring(0, string.indexOf('-'));  
    string = string.substring(string.indexOf('-') + 1);  
    _size++;
  }
  *size = _size;
  return modifiers;
}

int getKeyCode(String keyString) {
  for (int i = 0; i < NUM_OF_KEYS; i++) {
    if (keyString == duckyKeyMap[i].name) {
      return duckyKeyMap[i].code;
    }
  }
  return keymap[keyString[0]].usage;

}

int getModCode(String modString) {
    int size = 0;
    int modifierValue = 0;
    String * modifiers = splitModifiers(modString, &size);
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < NUM_OF_MODS; j++) {
        if (duckyModMap[j].name.equals(modifiers[i])) {
          modifierValue += duckyModMap[j].code;
          break;
        }
      }
    }
    return modifierValue;
}

bool keyExists(String keyString) {
    for (int i = 0; i < NUM_OF_KEYS; i++) {
      if (keyString == duckyKeyMap[i].name) {
        return true;
      }
    }
    return false;
}

bool modExists(String modString) {
  //still need to handle inject_mod
    for (int i = 0; i < NUM_OF_MODS; i++) {
      if (modString.startsWith( duckyModMap[i].name) ) {
        return true;
      }
    }
    return false;
}