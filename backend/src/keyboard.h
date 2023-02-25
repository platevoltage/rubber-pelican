#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "hidkeyboard.h"
#include "hidkeylayout.h"
//modifier keys

#define NUM_OF_MODS 7

#define MOD_COMMAND 8
#define MOD_WINDOWS 8
#define MOD_GUI 8
#define MOD_CTRL 1
#define MOD_OPTION 4
#define MOD_ALT 4
#define MOD_SHIFT 2

//regular keys

#define NUM_OF_KEYS 47

#define COMMAND 0xE3
#define WINDOWS 0xE3
#define GUI 0xE3
#define CTRL 0xE0
#define OPTION 0xE2
#define ALT 0xE2
#define SHIFT 0xE1
#define ESCAPE 0x29
#define ENTER 0x28
#define SPACE 0x2c
#define TAB 0x2b
#define UPARROW 0x52
#define DOWNARROW 0x51
#define LEFTARROW 0x50
#define RIGHTARROW 0x4f
#define UP 0x52
#define DOWN 0x51
#define LEFT 0x50
#define RIGHT 0x4f
#define PAGEUP 0x4b
#define PAGEDOWN 0x4e
#define HOME 0x4a
#define END 0x4d
#define INSERT 0x49
#define DELETE 0x4c
#define BACKSPACE 0x2a
#define PAUSE 0x48
#define BREAK 0x48
#define PRINTSCREEN 0x46
#define MENU 0x76
#define APP 0x76
#define F1 0x3a
#define F2 0x3b
#define F3 0x3c
#define F4 0x3d
#define F5 0x3e
#define F6 0x3f
#define F7 0x40
#define F8 0x41
#define F9 0x42
#define F10 0x43
#define F11 0x44
#define F12 0x45
#define CAPSLOCK 0x39
#define NUMLOCK 0x53
#define SCROLLLOCK 0x47

typedef struct {
    String name;
    int code;
} DuckyKeyMap;



extern HIDkeyboard keyboard;
extern RTC_DATA_ATTR bool keyboardActivated;
extern RTC_DATA_ATTR uint16_t VID;
extern RTC_DATA_ATTR uint16_t PID;
extern RTC_DATA_ATTR char manufacturer[15];
extern RTC_DATA_ATTR char product[15];
extern RTC_DATA_ATTR char serial[15];

void initializeKeyboard();
// void callback(String string); //duckyBlock uses this function to to decided what happens when a 

int getKeyCode(String keyString);
int getModCode(String modString);
bool keyExists(String keyString);
bool modExists(String modString);
bool randomCharExists(String modString);

#endif
