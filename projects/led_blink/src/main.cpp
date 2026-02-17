#include <Arduino.h>
static constexpr uint8_t internal_led = 2;
static constexpr uint8_t external_led = 4;

QueueHandle_t led_queue;

void toggleInternalLed(void* parameter){
  uint8_t counter=0;
  bool state=false;
  while(1){
    state=!state;
    digitalWrite(internal_led,state);
    if (!state){
      counter++;
    }
    if (counter>=5){
       uint8_t msg=1; 
      xQueueSend(led_queue,&msg,portMAX_DELAY); 
      counter=0;
    }
    vTaskDelay(500/portTICK_PERIOD_MS);
    }
  }

void toggleExternalLed(void* parameter){
  int received_msg=0;
  bool state=false;
  while(1){
    if (xQueueReceive(led_queue,&received_msg,portMAX_DELAY)==pdPASS){
      for (uint8_t i=0;i<(received_msg*2);i++){
        state=!state;
        digitalWrite(external_led,state);
        vTaskDelay(100/portTICK_PERIOD_MS);
      }
    }
  }
}

void setup(){
  led_queue=xQueueCreate(10,sizeof(uint8_t));
  pinMode(internal_led,OUTPUT);
  pinMode(external_led,OUTPUT);
  xTaskCreatePinnedToCore(toggleInternalLed, "Internal_Task",1024,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(toggleExternalLed, "EXternal_Task",1024,NULL,1,NULL,1);
}


void loop(){}

