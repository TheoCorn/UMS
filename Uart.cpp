//
// Created by theo on 5/9/2021.
//

#include "Uart.h"
#include "JsonSerializer.h"
#include <ArduinoJson>

Uart::Uart() {
//    serial = new Serial;
}

Uart::~Uart() {
//    delete serial;
}

bool Uart::begin(unsigned long boudRate){
    return Serial.begin(boudRate);
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

size_t Uart::write(const int *buffer, size_t size) {
    return Serial.write(buffer, size);
}

size_t Uart::write(const JsonDocument *doc) {
    js::serializeRet * sr = js::serializeDoc(doc);
    size_t ret = Serial.write(sr->buff, sr->bufLen);
    delete sr;
    return ret;
}
