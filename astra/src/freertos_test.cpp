#include <Arduino.h>
#define pin2 2

enum{btn_UP,btn_DOWN,btn_ATK};

TaskHandle_t indicatorTask=NULL;
QueueHandle_t msgQUEUE=NULL;
QueueHandle_t btnQUEUE=NULL;
SemaphoreHandle_t serialMUTEX=NULL;


static void vIndicatorLight(void *pvParameter){
    uint8_t pin=(uint32_t)pvParameter;
    for(;;){
        digitalWrite(pin, !digitalRead(pin));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void vSendMessage(void* pvParameter){
    char* msg=(char*)pvParameter;
    for(;;){
        if (xQueueSend(msgQUEUE,&msg,pdMS_TO_TICKS(1000))==pdPASS){};
        vTaskDelay(pdMS_TO_TICKS(100));
    }
};

static void vReceiveMessage(void* pvParameter){
    char* rec_msg=NULL;
    for(;;){
        if (xQueueReceive(msgQUEUE,&rec_msg,portMAX_DELAY)==pdPASS){
            if(xSemaphoreTake(serialMUTEX,portMAX_DELAY)==pdPASS){
                Serial.println(rec_msg);
                xSemaphoreGive(serialMUTEX);
            }
        }   
        
    }
}

static void vButtonPressed(void* pvParameter){
    uint8_t button=(uint32_t)pvParameter;
    for(;;){
        if (xQueueSend(btnQUEUE,&button,pdMS_TO_TICKS(100))==pdPASS){};
        vTaskDelay(pdMS_TO_TICKS(100));
    }   
}

static void vButtonReleased(void* pvParameter){
    uint8_t receive_BTN=0;
    for (;;){
    if (xQueueReceive(btnQUEUE,&receive_BTN,portMAX_DELAY)==pdPASS){
        if(xSemaphoreTake(serialMUTEX,portMAX_DELAY)==pdPASS){
            Serial.println("Button Pressed!");
            xSemaphoreGive(serialMUTEX);
            }
        } 
    }
}

void setup(){ 
    pinMode(pin2,OUTPUT);
    Serial.begin(115200);
    if (serialMUTEX==NULL) serialMUTEX=xSemaphoreCreateMutex();   
    if (msgQUEUE==NULL) msgQUEUE=xQueueCreate(10,sizeof(char*));
    if (btnQUEUE==NULL) btnQUEUE=xQueueCreate(10,sizeof(uint8_t));
    if(indicatorTask==NULL) xTaskCreate(vIndicatorLight,"IndicatorTask",1024,(void*)pin2,1,&indicatorTask);
    xTaskCreate(vSendMessage,"SendTask",1024,(void*)"Hello",1,NULL);
    xTaskCreate(vReceiveMessage,"ReceiveTask",2048,NULL,1,NULL);
    xTaskCreate(vButtonPressed,"ButtonPressed",1024,(void*)btn_UP,1,NULL);
    xTaskCreate(vButtonReleased,"ButtonReleased",2048,NULL,1,NULL);

}

void loop(){};