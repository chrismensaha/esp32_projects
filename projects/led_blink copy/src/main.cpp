#include <Arduino.h>

static constexpr uint8_t LedPin = 2;



void setup() {
  
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
  
}
