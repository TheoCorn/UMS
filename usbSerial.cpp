
#include "usbSerial.h"
#include "JsonSerializer.h"
#include <ArduinoJson.h>
#include "asciiMakros.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Error.h"


#define UART_BAUD 115200

usbSerial::usbSerial() : SerialCom() {
    Serial.begin(UART_BAUD);
}

usbSerial::~usbSerial() = default;

bool usbSerial::begin(){
    Serial.begin(UART_BAUD);
    return true;
}

std::size_t usbSerial::available() {
    return Serial.available();
}

void usbSerial::read(char *c) {
    *c = Serial.read();
}

size_t usbSerial::write(uint8_t c) {
    return Serial.write(c);
}

size_t usbSerial::write(uint8_t *buffer, size_t size) {
    size_t ret = Serial.write(buffer, size);
    Serial.write(ETX);
    return ret;

}

size_t usbSerial::write(JsonDocument* doc) {

    js::serializeRet * sr = js::serializeDoc(doc);

    size_t ret = Serial.write(sr->buff, sr->bufLen);
    delete sr;
    return ret;
}

size_t usbSerial::write(error::Error* error) {
    js::serializeRet * sr = js::serializeError(error);
    size_t ret = Serial.write(sr->buff, sr->bufLen);
    delete sr;
    return ret;
}

void usbSerial::flush(){
    Serial.flush();
}

bool usbSerial::hasConnectedDevice() {
    return connected;
}

void usbSerial::startConnectionCheck(int duration) {
    ConnCheckInfo* info = new ConnCheckInfo(duration, &connected, &UartConnCheckHandle);
    xTaskCreatePinnedToCore(connectionCheckTask, "UartSender", 8196, info, 2, &UartConnCheckHandle, 0);
//    configASSERT(UartConnCheckHandle);
}

void usbSerial::connectionCheckTask(void* connInfo) {
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
