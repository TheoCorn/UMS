//
// Created by theo on 5/9/2021.
//

#include "Uart.h"
#include "JsonSerializer.h"
#include <ArduinoJson.h>
#include "asciiMakros.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define UART_BAUD 115200

Uart::Uart() : SerialCom() {
    Serial.begin(UART_BAUD);
//    serial = new Serial;
}

Uart::~Uart() {
//    delete serial;
}

bool Uart::begin(){
    Serial.begin(UART_BAUD);
    return true;
}

int Uart::available() {
    return Serial.available();
}

void Uart::read(char *c) {
    *c = Serial.read();
}

size_t Uart::write(const uint8_t c) {
    return Serial.write(c);
}

size_t Uart::write(const uint8_t *buffer, size_t size) {
    return Serial.write(buffer, size);
}

size_t Uart::write(JsonDocument* doc) {
    js::serializeRet * sr = js::serializeDoc(doc);
    size_t ret = Serial.write(sr->buff, sr->bufLen);
    delete sr; 
    return ret;
}

void Uart::flush(){
    Serial.flush();
}

void Uart::startConnectionCheck(int duration) {
    ConnCheckInfo* info = new ConnCheckInfo(duration, &connected);
    xTaskCreate(connectionCheckTask, "UartSender", 2048, info, 2, &UartConnCheckHandle);
    configASSERT(UartConnCheckHandle);
}

void Uart::connectionCheckTask(void* connInfo) {
    ConnCheckInfo* info = (ConnCheckInfo*)connInfo;
    long endTime = millis() + (long)(info->duration);
    TaskHandle_t UartReaderHandle = NULL;

    while(millis() < endTime){
        Serial.write(static_cast<char>(STX));
        vTaskDelay(50);
        if(Serial.available() > 0) {
            *(info->connected) = true;
            Serial.flush();
            break;
        }
    }
    delete info;
}
