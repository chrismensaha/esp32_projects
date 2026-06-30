#include <Arduino.h>
#define PIN2 2
#define btnPIN 17

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

static void vButtonISR(void){
    BaseType_t pxHigherPriorityTaskWoken=pdFALSE;
    uint8_t button=btn_UP;
    xQueueSendFromISR(btnQUEUE,&button,&pxHigherPriorityTaskWoken);
    if (pxHigherPriorityTaskWoken){
        portYIELD_FROM_ISR();
    }
}

static void vButtonReleasedISR(void){
    BaseType_t pxHigherPriorityTaskWoken=pdFALSE;
    uint8_t receive_BTN=0;   
    xQueueReceiveFromISR(btnQUEUE,&receive_BTN,&pxHigherPriorityTaskWoken);
    xSemaphoreTakeFromISR(serialMUTEX,&pxHigherPriorityTaskWoken);
    Serial.println("Button Pressed!");
    xSemaphoreGiveFromISR(serialMUTEX,&pxHigherPriorityTaskWoken);        
}

void setup(){ 
    pinMode(PIN2,OUTPUT);
    Serial.begin(115200);
    if (serialMUTEX==NULL) serialMUTEX=xSemaphoreCreateMutex();   
    if (msgQUEUE==NULL) msgQUEUE=xQueueCreate(10,sizeof(char*));
    if (btnQUEUE==NULL) btnQUEUE=xQueueCreate(10,sizeof(uint8_t));
    if(indicatorTask==NULL) xTaskCreate(vIndicatorLight,"IndicatorTask",1024,(void*)PIN2,1,&indicatorTask);
    xTaskCreate(vSendMessage,"SendTask",1024,(void*)"Hello",1,NULL);
    xTaskCreate(vReceiveMessage,"ReceiveTask",2048,NULL,1,NULL);
    attachInterrupt(digitalPinToInterrupt(btnPIN),vButtonISR,FALLING);
    attachInterrupt(digitalPinToInterrupt(btnPIN),vButtonReleasedISR,RISING);

}

void loop(){};