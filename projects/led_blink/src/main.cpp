#include <Arduino.h>
static constexpr uint8_t internal_led = 2;
static constexpr uint8_t external_led = 4;
unsigned long internal_previous_time=0;
unsigned long external_previous_time=0;
bool internal_state=0;
bool external_state=0;


void toggleInternalLed(void* parameter){
  while(1){
    internal_state=!internal_state;
    digitalWrite(internal_led,internal_state);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void toggleExternalLed(void* parameter){
  while(1){
    external_state=!external_state;
    digitalWrite(external_led,external_state);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void setup(){
  pinMode(internal_led,OUTPUT);
  pinMode(external_led,OUTPUT); 
  xTaskCreatePinnedToCore(toggleInternalLed, "Internal LED Task",1024,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(toggleExternalLed, "EXternal LED Task",1024,NULL,1,NULL,1);
}

void loop() {

}



