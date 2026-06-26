
#include <Arduino.h>
#include <Encoder.h>

Encoder myEnc(52, 53);

const char* DIRECTION[8] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW"};


const int STEP_SIZE = 300;
static long lastIndex = 1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  long count = myEnc.read();
  int step = count / STEP_SIZE;
  int index = ((step % 8) + 8) % 8;

  if (index != lastIndex) {
    Serial.println(DIRECTION[index]);
    lastIndex = index;
  }
}