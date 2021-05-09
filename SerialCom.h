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

    virtual bool begin(unsigned long boudRate);
    virtual int available();
    virtual bool read(char* c);
    virtual size_t write(const uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual size_t write(const JsonDocument * doc);
    virtual void flush();

};


#endif //UMDWITHCLASS3_SERIALCOM_H
