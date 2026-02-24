#include <Arduino.h>
static constexpr uint8_t INTERNAL_LED = 2;
static constexpr uint8_t EXTERNAL_LED = 21;
static constexpr uint8_t BUTTON_PIN=23;

QueueHandle_t LED_QUEUE;


void IRAM_ATTR pushed(){
  uint8_t BLINK=5;
  xQueueSendFromISR(LED_QUEUE,&BLINK,NULL);
}

void aliveIndicator(void* parameter){
  bool state=false;
  while(1){
    state=!state;
    digitalWrite(INTERNAL_LED,state);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    }
  }

void alarmON(void* parameter){
  uint8_t received_msg=0;
  bool state=false;
  while(1){
    if (xQueueReceive(LED_QUEUE,&received_msg,portMAX_DELAY)==pdPASS){
      for (uint8_t i=0;i<(received_msg*2);i++){
        state=!state;
        digitalWrite(EXTERNAL_LED,state);
        vTaskDelay(100/portTICK_PERIOD_MS);
      }
    }
  }
}

void setup(){
  LED_QUEUE=xQueueCreate(10,sizeof(uint8_t));
  pinMode(INTERNAL_LED,OUTPUT);
  pinMode(EXTERNAL_LED,OUTPUT);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  xTaskCreatePinnedToCore(aliveIndicator,"Is_Alive",1024,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(alarmON,"Alarm_On",1024,NULL,1,NULL,1);
  attachInterrupt(BUTTON_PIN,pushed,FALLING);
}

void loop(){}

