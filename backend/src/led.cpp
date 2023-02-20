#include "led.h"


#define LED_PIN 8
#define LED_COUNT 1
CRGB leds[LED_COUNT];

void initializeLED() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(64);
  leds[0] = CRGB::Black;
  FastLED.show();
}