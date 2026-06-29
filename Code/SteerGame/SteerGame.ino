#include <Arduino.h>
#include <Encoder.h>
#include <FastLED.h>

// 52 = green / 53 = white
Encoder myEnc(52, 53);

#define NUM_LEDS 8
#define DATA_PIN 39

CRGB leds[NUM_LEDS];

bool visited[NUM_LEDS] = { false };

const int STEP_SIZE = 600;
static long lastDir = -1;
int direction = 0;
int target = 0;
int score = 0;

int state = 0;

// function that returns T/F depending on the visit status of the compass LEDs.
bool allVisited() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (!visited[i]) return false;
  }
  return true;
}

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

  if (state == 0) {  // Unlock wheel by steering
    if (direction != lastDir) {
      visited[direction] = true;
      leds[direction] = CRGB::Blue;
      FastLED.show();
      lastDir = direction;
    }
    if (allVisited()) {
      state++;
    }
  }
  if (state == 1) {
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
  if (state == 2) {
    FastLED.clear();
    FastLED.show();

    // clear map path
    for (int i = 1; i < 32; i++) {
      digitalWrite(i, LOW);
      delay(50);
    }
    delay(500);

    // light up map path
    for (int i = 1; i < 32; i++) {
      digitalWrite(i, HIGH);
      delay(50);
    }
  
  }
}
