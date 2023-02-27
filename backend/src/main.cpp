#include <Arduino.h>
#include "server.h"
#include "led.h"
#include "flash.h"
#include "ducky.h"

#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1

esp_reset_reason_t BootReason = esp_reset_reason();

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  Serial1.print("Reset/Boot Reason was: "); 
  Serial1.println( BootReason );
  
  esp_reset_reason_t reason = esp_reset_reason();

  switch (reason) {
      case ESP_RST_UNKNOWN:
        Serial1.println("Reset reason can not be determined");
      break;

      case ESP_RST_POWERON:
        Serial1.println("Reset due to power-on event");
      break;

      case ESP_RST_EXT:
        Serial1.println("Reset by external pin (not applicable for ESP32)");
      break;

      case ESP_RST_SW:
        Serial1.println("Software reset via esp_restart");
      break;

      case ESP_RST_PANIC:
        Serial1.println("Software reset due to exception/panic");
      break;

      case ESP_RST_INT_WDT:
        Serial1.println("Reset (software or hardware) due to interrupt watchdog");
      break;

      case ESP_RST_TASK_WDT:
        Serial1.println("Reset due to task watchdog");
      break;

      case ESP_RST_WDT:
        Serial1.println("Reset due to other watchdogs");
      break;                                

      case ESP_RST_DEEPSLEEP:
        Serial1.println("Reset after exiting deep sleep mode");
      break;

      case ESP_RST_BROWNOUT:
        Serial1.println("Brownout reset (software or hardware)");
      break;
      
      case ESP_RST_SDIO:
        Serial1.println("Reset over SDIO");
      break;
      
      default:
      break;
  }
  
  pinMode(13, INPUT_PULLUP);
  initializeLED();
  if (!digitalRead(13) && startOnLineBoot == 0) {   //opens system drive if button pushed on boot
    mountSystemDrive();                             //execution is halted until button is pushed again and system reboots
  }

  randomSeed(millis());
  
  if (keyboardActivated) initializeKeyboard();
  initializeFlash();
  initializeSystemFS();

  //read preferences
  String preferences = readFile( "/preferences.txt"); 
  int size;
  DuckyCommand * prefs = splitByLine(preferences, &size) ;

  for (int i = 0; i < size; i++) {
    if (prefs[i].instruction.equals("SSID")) {
      ssid = prefs[i].parameter;
    }
    else if (prefs[i].instruction.equals("PASSWORD")) {
      password = prefs[i].parameter;
    }
    else if (prefs[i].instruction.equals("BONJOUR")) {
      bonjourName = prefs[i].parameter;
    }
  }
  
  String string = readFile( "/.cache.txt");
 
  if(startOnLineBoot>0) resumeDuckyScript(string, startOnLineBoot);

  xTaskCreatePinnedToCore(serverTask, "Server Task", 10000, NULL, 1, NULL, 1); //webserver gets it's own task

}

void loop() {
  // put your main code here, to run repeatedly:

}
