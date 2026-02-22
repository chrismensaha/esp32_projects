#include <Arduino.h>
#include <WiFi.h>

void wifiTask(void* parameter){
    WiFi.mode(WIFI_STA);
    for(;;){
        Serial.print("ESP32 MAC Address: ");
        Serial.println(WiFi.macAddress());
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

void setup(){
    Serial.begin(115200);
    xTaskCreatePinnedToCore(wifiTask,"Wifi_Task",2048,NULL,1,NULL,0);
}


void loop(){}