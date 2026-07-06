#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Extensions/Sprite.h>


#define SERIAL_BAUD 115200
#define BUTTON_PIN 21
#define INTERNAL_LED 2
#define HUMIDITY_SENSOR 17
#define ULTRASONIC_DISTANCE_SENSOR 23
#define ulBitsToClearOnEntry 0x00
#define ulBitsToClearOnExit 0xFFFFFFFF

QueueHandle_t telemetryQueue=NULL;
QueueHandle_t stringQueue=NULL;
QueueHandle_t controlQueue=NULL;
TimerHandle_t debounceTimer=NULL;

typedef enum dataType : uint8_t {
    MOISTURE,
    TEMPERATURE,
    PRESSURE,
    FREQUENCY,
    TIME
};

typedef struct telemetry_t{   
    dataType Type : 4;
    union{
        int integerVal;
        float floatVal;
    }value;
}Telemetry;

static void vHeartBeat(void* pvParameters){
    uint8_t pin=(uint32_t)pvParameters;
    for(;;){   
        digitalWrite(BUTTON_PIN,!digitalRead(BUTTON_PIN));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void vMainControl(void* pvParameters){

    for(;;){
        uint8_t controlCycle=0;  
        uint8_t screenNumber=0;
        enum screenState : uint8_t {SCREEN_HOME,SCREEN_SENSORS,SCREEN_LOGS,SCREEN_MAX};
        if (xQueueReceive(controlQueue,&controlCycle,portMAX_DELAY)==pdPASS){
            if (xQueueReceive(controlQueue,&screenNumber,pdMS_TO_TICKS(10))==pdPASS){      
            switch (screenNumber){
                case SCREEN_HOME:
                    


                    break;
                case SCREEN_SENSORS:
                    
                    break;            
                case SCREEN_LOGS:
                    
                    break;
                default:
                    break;
                }
                }
            }
        }
    }



static void vCreateDisplay(void* pvParameters){

}
static void vDisplayControl(void* pvParameters){
    for(;;){
        char* stringMSG=0;
        uint8_t screenNumber=0;
        Telemetry telemetry;
        if (xQueueReceive(telemetryQueue,&telemetry,0)==pdPASS){
            switch (telemetry.Type){           
                case MOISTURE:
                    screenNumber=0;
                    Serial.printf("\nThe absolute humidity is %.2f kg/m^2. \n",telemetry.value.floatVal);
                    xQueueSend(controlQueue,&screenNumber,0);
                    break;
                case TEMPERATURE:
                    screenNumber=1;
                    Serial.printf("\nThe temperature is %.2f Degrees Celsius. \n",telemetry.value.floatVal);
                    xQueueSend(controlQueue,&screenNumber,0);
                    break;
                case PRESSURE:
                    screenNumber=2;
                    Serial.printf("\nThe pressure is %.2f N/m^2. \n",telemetry.value.floatVal);
                    xQueueSend(controlQueue,&screenNumber,0);
                    break;

                case FREQUENCY:
                    screenNumber=3;
                    Serial.printf("\nThe frequency is %.2f Hz. \n",telemetry.value.integerVal);
                    xQueueSend(controlQueue,&screenNumber,0);
                    break;
                case TIME:  
                    screenNumber=4;
                    xQueueSend(controlQueue,&screenNumber,0);
                    break;               
                default:
                    screenNumber=5;
                    Serial.printf("\nIncorrect Type! Error Found In TypeID: %d\n",telemetry.Type);
                    xQueueSend(controlQueue,&screenNumber,0);
                    break;
                }
            }           
            if (xQueueReceive(stringQueue,&stringMSG,pdMS_TO_TICKS(10))==pdPASS){
                Serial.println(stringMSG);   
            }
    }
}

static void vButtonISR(void){
    BaseType_t pxHigherPriorityTaskWoken=pdFALSE;
    xTimerStartFromISR(debounceTimer,&pxHigherPriorityTaskWoken);

    if (pxHigherPriorityTaskWoken){
        portYIELD_FROM_ISR();
    }
}

static void vDebounceCallback(TimerHandle_t Timer){
    uint8_t buttonState=digitalRead(BUTTON_PIN);
    uint8_t buttonEvent=(buttonState) ? 1:0;
    xTaskNotify(vButton,buttonEvent,eSetValueWithOverwrite);
}

static void vButton(void* pvParameters){
    uint32_t screenCheck=0;
    enum {CHANGE_SCREEN_TRUE,CHANGE_SCREEN_FALSE};
    if (xTaskNotifyWait(ulBitsToClearOnEntry,ulBitsToClearOnExit,&screenCheck,portMAX_DELAY)==pdPASS){
        uint8_t controlCycle=(screenCheck==1) ? CHANGE_SCREEN_TRUE:CHANGE_SCREEN_FALSE;
        xQueueSend(controlQueue,&controlCycle,0);
    }
}

static void v(void* pvParameters){
    
}

void setup(){
    Serial.begin(SERIAL_BAUD);
    pinMode(BUTTON_PIN,INPUT_PULLUP);
    pinMode(INTERNAL_LED,OUTPUT);

    if (telemetryQueue==NULL) telemetryQueue=xQueueCreate(10,sizeof(telemetry_t));
    if (stringQueue==NULL) stringQueue=xQueueCreate(10,sizeof(telemetry_t));
    if (controlQueue==NULL) controlQueue=xQueueCreate(10,sizeof(uint32_t));
    
    xTaskCreate(vDisplayControl,"GateKeeper Task",2048,NULL,1,NULL);
    xTaskCreate(vMainControl,"Main Control Task",2048,NULL,2,NULL);
    xTaskCreate(vDebounceCallback,"Debounce Callback",2048,NULL,2,NULL);
    xTaskCreate(vButton,"Button Task",2048,NULL,2,NULL);
    
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),vButtonISR,CHANGE);
    

}

void loop(){}