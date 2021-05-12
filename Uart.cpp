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

size_t Uart::write(const JsonDocument *doc) {
    js::serializeRet * sr = js::serializeDoc(doc);
    size_t ret = Serial.write(sr->buff, sr->bufLen);
    delete sr; 
    return ret;
}

void Uart::flush(){
    Serial.flush();
}

void Uart::startConnectionCheck(int duration) {
    xTaskCreate(connectionCheckTask, "UartSender", 2048, NULL, 2, &UartSenderHandle);
    configASSERT(UartSenderHandle);
}

void Uart::connectionCheckTask() {
    Serial.write(static_cast<char>(stx));
    xTaskCreate(readTask, "UartReader", 2048, NULL, 2, &UartReaderTask);
    configASSERT(UartReaderTask);
    vTaskDelay(500);
    if(UartReaderTask != NULL) vTaskDelete(UartReaderTask);

}

void Uart::readTask(bool* connected) {
    if(Serial.read() == ACK) *connected = true;
}
