//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_SERIALCOM_H
#define UMDWITHCLASS3_SERIALCOM_H

#include <ArduinoJson.h>
#include "JsonSerializer.h"

class SerialCom {
public:

    SerialCom();
    ~SerialCom();

    virtual bool begin() = 0;
    virtual int available() = 0;
    virtual bool read(char* c) = 0;
    virtual size_t write(const uint8_t c) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    virtual size_t write(JsonDocument * doc) = 0;
    virtual void flush();
    virtual void startConnectionCheck(int duration) = 0;
    virtual bool hasConnectedDevice() = 0;

};


#endif //UMDWITHCLASS3_SERIALCOM_H
