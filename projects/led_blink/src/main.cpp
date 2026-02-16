#include <Arduino.h>
static constexpr uint8_t internal_led = 2;
static constexpr uint8_t external_led = 4;
unsigned long internal_previous_time=0;
unsigned long external_previous_time=0;
bool internal_state=0;
bool external_state=0;

void setup(){
  pinMode(internal_led,OUTPUT);
  pinMode(external_led,OUTPUT); 
  xTaskCreatePinnedToCore();
  
}

void toggleInternalLed(void* parameter){
  while(1){
    internal_state=!internal_state;
    digitalWrite(internal_led,internal_state);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void loop() {
  unsigned long current_time=millis();
  if (current_time-external_previous_time>=1000){
    external_state=!external_state;
    digitalWrite(external_led,external_state);
    external_previous_time+=1000;
  }

}


