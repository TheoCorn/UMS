
#include "Uart.h"
#include "JsonSerializer.h"
#include <ArduinoJson.h>
#include "asciiMakros.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Error.h"


#define UART_BAUD 115200

Uart::Uart() : SerialCom() {
    Serial.begin(UART_BAUD);
}

Uart::~Uart() = default;

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

size_t Uart::write(error::Error* error) {
    js::serializeRet * sr = js::serializeError(error);
    size_t ret = Serial.write(sr->buff, sr->bufLen);
    delete sr;
    return ret;
}

void Uart::flush(){
    Serial.flush();
}

bool Uart::hasConnectedDevice() {
    return connected;
}

void Uart::startConnectionCheck(int duration) {
    ConnCheckInfo* info = new ConnCheckInfo(duration, &connected, &UartConnCheckHandle);
    xTaskCreatePinnedToCore(connectionCheckTask, "UartSender", 8196, info, 2, &UartConnCheckHandle, 0);
//    configASSERT(UartConnCheckHandle);
}

void Uart::connectionCheckTask(void* connInfo) {
    ConnCheckInfo* info = (ConnCheckInfo*)connInfo;
    long endTime = millis() + (long)(info->duration);

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

    vTaskDelete(*(info->taskHandle));
}



//char* getString4Display(){
//    return nullptr;
//}
