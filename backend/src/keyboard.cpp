#include "keyboard.h"


HIDkeyboard keyboard;

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
    {"F12", F12}
};

void initializeKeyboard() {
  keyboard.begin();
}

int getKeycode(String keyString) {
  for (int i = 0; i < NUM_OF_KEYS; i++) {
    if (keyString == duckyKeyMap[i].name) {
      return duckyKeyMap[i].code;
    }
  }
  return keymap[keyString[0]].usage;

}