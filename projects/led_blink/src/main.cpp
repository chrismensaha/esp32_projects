#include <Arduino.h>
static constexpr uint8_t internal_led = 2;
static constexpr uint8_t external_led = 4;
unsigned long previous_time=0;

void setup(){
  pinMode(internal_led,OUTPUT);
  pinMode(external_led,OUTPUT); 
  
}

void loop() {
  unsigned long current_time=millis();
  if (current_time-previous_time>=500){
    digitalWrite(internal_led,HIGH);
    digitalWrite(external_led,HIGH);
    previous_time+500;
  }

}


