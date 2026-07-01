/* 
SHIP LED TEST CODE

*/

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 8
#define DATA_PIN 39

CRGB leds[NUM_LEDS];

void setup() {
  // Cannot have serial, otherwise pin 1 stays lit
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  for (int i = 4; i < 34; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // order for the Compass LEDs to flash
  int order[] = { 2, 3, 4, 5, 6, 7, 0, 1, 2 };


  // light up map path
  for (int i = 4; i < 34; i++) {
    digitalWrite(i, HIGH);
    delay(50);
  }

  // Flashing compass lights
  for (int i = 0; i < 9; i++) {
    leds[order[i]] = CRGB::Blue;
    FastLED.show();
    delay(200);
    // FastLED.clear();
    // FastLED.show();
    // delay(200);
  }
  delay(10000);

  // clear map path
  for (int i = 4; i < 24; i++) {
    digitalWrite(i, LOW);
    delay(50);
  }
}
