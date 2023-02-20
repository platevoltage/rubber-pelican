#include <Arduino.h>
#include "server.h"
#include <FastLED.h>

#define LED_PIN 8
#define LED_COUNT 1
CRGB leds[LED_COUNT];



#define BAUD 115200       // Any baudrate from 300 to 115200
#define RXPIN 33         // GPIO 33 => RX for Serial1
#define TXPIN 35         // GPIO 35 => TX for Serial1



void setup() {
  // put your setup code here, to run once:
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);

  initializeKeyboard();
  xTaskCreate(serverTask, "Server Task", 10000, NULL, 1, NULL); //webserver gets it's own task
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(64);
  leds[0] = CRGB::Red;
  FastLED.show();

}

void loop() {
  // put your main code here, to run repeatedly:
}
