#include <Arduino.h>
#define pin2 2

enum{btn_UP,btn_DOWN,btn_ATK};

TaskHandle_t task1=NULL;
QueueHandle_t msg_QUEUE=NULL;
QueueHandle_t btn_QUEUE=NULL;

void vIndicatorLight(void *pvParameter){
    uint8_t pin=(uint32_t)pvParameter;
    pinMode(pin,OUTPUT);
    for(;;){
        digitalWrite(pin, !digitalRead(pin));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vSendMessage(void* pvParameter){
    char* msg=(char*)pvParameter;
    for(;;){
        if (xQueueSend(msg_QUEUE,&msg,pdMS_TO_TICKS(1000))==pdPASS){};
        vTaskDelay(pdMS_TO_TICKS(100));
    }
};

void vReceiveMessage(void* pvParameter){
    char* rec_msg=NULL;
    for(;;){
        if (xQueueReceive(msg_QUEUE,&rec_msg,portMAX_DELAY)==pdPASS){
            Serial.println(rec_msg);
        }   
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vButtonPressed(void* pvParameter){
    uint8_t button=(uint32_t)pvParameter;
    for(;;){
        if (xQueueSend(btn_QUEUE,&button,pdMS_TO_TICKS(100))==pdPASS){};
        vTaskDelay(pdMS_TO_TICKS(100));
    }   
}

void vButtonReleased(void* pvParameter){
    uint8_t receive_BTN=0;
    for (;;){
    if (xQueueReceive(btn_QUEUE,&receive_BTN,portMAX_DELAY)==pdPASS){
        Serial.println("Button Pressed!");
    }
    vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void setup(){
    Serial.begin(115200);
    if (msg_QUEUE==NULL) msg_QUEUE=xQueueCreate(10,sizeof(char*));
    if (btn_QUEUE==NULL) btn_QUEUE=xQueueCreate(10,sizeof(uint8_t));
    if(task1==NULL) xTaskCreate(vIndicatorLight,"IndicatorTask",1024,(void*)pin2,1,&task1);
    xTaskCreate(vSendMessage,"SendTask",1024,(void*)"Hello",1,NULL);
    xTaskCreate(vReceiveMessage,"ReceiveTask",2048,NULL,1,NULL);
    xTaskCreate(vButtonPressed,"ButtonPressed",1024,(void*)btn_UP,1,NULL);
    xTaskCreate(vButtonReleased,"ButtonReleased",2048,NULL,1,NULL);
}

void loop(){};