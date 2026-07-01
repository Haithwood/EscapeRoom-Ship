#include <Arduino.h>
#include <Encoder.h>
#include <FastLED.h>

// 52 = green / 53 = white
Encoder myEnc(52, 53);

#define NUM_LEDS 8
#define DATA_PIN 39

CRGB leds[NUM_LEDS];

const int STEP_SIZE = 600;
static long lastDir = -1;

int direction = 0;
int target = 0;

int score = 0;

void pick_target() {
  target = random(0, 7);
  if (target == direction) {
    target = (direction + 3) % 8;
  }
}

void update_leds() {
  FastLED.clear();
  leds[direction] = CRGB::Blue;
  leds[target] = CRGB::Green;
  FastLED.show();
}


void setup() {
  // Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  for (int i = 1; i < 32; i++) {
    pinMode(i, OUTPUT);
  }

  randomSeed(analogRead(0));
}

void loop() {

  long count = myEnc.read();
  int step = count / STEP_SIZE;
  direction = ((step % 8) + 8) % 8;

  // Movement update
  if (direction != lastDir) {
    // Serial.println(DIRECTION[direction]);
    lastDir = direction;

    // Success condition
    if (direction == target) {
      pick_target();
      score++;

      if (score >= 31) {
        score = 31;
        state++;
      }

      digitalWrite(score, HIGH);

      // Serial.println(score);
    }

    update_leds();
  }
}
