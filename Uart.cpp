//
// Created by theo on 5/9/2021.
//

#include "Uart.h"
#include "JsonSerializer.h"
#include <ArduinoJson.h>
#include "asciiMakros.h"

#define UART_BAUD 115200

Uart::Uart() {
    begin();
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

bool Uart::read(char *c) {
    *c = Serial.read();
}

size_t Uart::write(const uint8_t c) {
    return Serial.write(c);
}

size_t Uart::write(const uint8_t *buffer, size_t size) {
    return Serial.write(buffer, size);
}

size_t Uart::write(JsonDocument *doc) {
    js::serializeRet * sr = js::serializeDoc(doc);
    size_t ret = Serial.write(sr->buff, sr->bufLen);
    delete sr; 
    return ret;
}

void Uart::flush(){
    Serial.flush();
}

void Uart::startConnectionCheck(int duration) {
    xTaskCreate(connectionCheckTask, "UartSender", 2048, NULL, 2, &UartConnCheckHandle);
    configASSERT(UartSenderHandle);
}

void Uart::connectionCheckTask(void* duration) {
    long endTime = millis() + (long)(*(int*)duration);
    TaskHandle_t UartReaderHandle = NULL;
    xTaskCreate(readTask, "UartReader", 2048, NULL, 2, &UartReaderHandle);
    configASSERT(UartReaderHandle);

    while(millis() < endTime){
        Serial.write(static_cast<char>(stx));
        vTaskDelay(50);
    }

    if(UartReaderTask != NULL) vTaskDelete(UartReaderTask);

}

void Uart::readTask(void* connected) {
    if(Serial.read() == ACK) *((bool*)connected) = true;
}
