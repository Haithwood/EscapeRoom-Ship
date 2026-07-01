#include <Arduino.h>
#include <Encoder.h>
#include <FastLED.h>

// 2 = green / 3 = white
Encoder myEnc(2, 3);

#define NUM_LEDS 8
#define DATA_PIN 39

CRGB leds[NUM_LEDS];

bool visited[NUM_LEDS] = { false };

const int STEP_SIZE = 600;
static long lastDir = -1;
int direction = 0;
int greenTarget = 0;
int redTarget = 0;
int score = 3;

int state = 0;

int maglockPin = 38;

// function that returns T/F depending on the visit status of the compass LEDs.
bool allVisited() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (!visited[i]) return false;
  }
  return true;
}

void pick_green_target() {
  greenTarget = random(0, 7);
  if (greenTarget == direction || greenTarget == redTarget) {
    greenTarget = (direction + 3) % 8;
  }
}

void pick_red_target() {
  redTarget = random(0, 7);
  if (redTarget == direction || redTarget == greenTarget) {
    redTarget = (direction + 3) % 8;
  }
}

void update_leds() {
  FastLED.clear();
  leds[direction] = CRGB::White;
  leds[redTarget] = CRGB::Red;
  leds[greenTarget] = CRGB::Green;
  FastLED.show();
}


void setup() {
  // Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  pinMode(maglockPin, OUTPUT);
  digitalWrite(maglockPin, LOW);

  for (int i = 4; i < 34; i++) {
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
      lastDir = direction;


      if (direction == redTarget) {
        digitalWrite(score, LOW);
        pick_red_target();
        score--;
        if (score <= 3) {
          score = 3;
        }
        digitalWrite(score, HIGH);
      }

      // Success condition
      if (direction == greenTarget) {
        pick_green_target();
        pick_red_target();
        score++;

        if (score >= 34) {
          score = 34;
          state++;
        }
        digitalWrite(score, HIGH);
      }



      update_leds();
    }
  }
  if (state == 2) {
    FastLED.clear();
    FastLED.show();

    digitalWrite(maglockPin, HIGH);

    // clear map path
    for (int i = 4; i < 34; i++) {
      digitalWrite(i, LOW);
      delay(50);
    }
    delay(500);

    // light up map path
    for (int i = 4; i < 34; i++) {
      digitalWrite(i, HIGH);
      delay(50);
    }


  }
}
