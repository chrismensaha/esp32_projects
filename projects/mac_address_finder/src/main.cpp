#include <Arduino.h>
#include "esp_mac.h"

#define SERIAL_BAUD          115200
#define SERIAL_INIT_TIMEOUT  3000
#define REPORT_INTERVAL      5000  
#define TASK_STACK_SIZE      3072
#define TASK_PRIORITY        1
#define TARGET_CORE          0

void identityTask(void *pvParameters){
    uint8_t mac[6];
    for (;;){
        if (esp_read_mac(mac,ESP_MAC_WIFI_STA)==ESP_OK){
            Serial.printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);            
            UBaseType_t high_water_mark=uxTaskGetStackHighWaterMark(NULL);
            Serial.printf("Stacks: %u bytes\n",(unsigned int)high_water_mark);
        }
        vTaskDelay(pdMS_TO_TICKS(REPORT_INTERVAL));
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD);   
    uint32_t start_attempt=millis();
    while (!Serial && (millis()-start_attempt<SERIAL_INIT_TIMEOUT));  
    xTaskCreatePinnedToCore(identityTask,"ID_Task",TASK_STACK_SIZE,NULL,TASK_PRIORITY,NULL,TARGET_CORE);
    vTaskDelete(NULL);
}

void loop(){} 

// Board1: 44:1D:64:F4:55:A8 Stack: 368 bytes
// Board2:  MAC: 44:1D:64:F4:FB:EC Stack: 416 bytes