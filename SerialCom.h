//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_SERIALCOM_H
#define UMDWITHCLASS3_SERIALCOM_H

#include <ArduinoJson.h>

class SerialCom {

    SerialCom();
    ~SerialCom();

    virtual bool begin(unsigned long boudRate);
    virtual int available();
    virtual bool read(char* c);
    virtual size_t write(const uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t);
    virtual size_t write(const JsonDocument * doc);

};


#endif //UMDWITHCLASS3_SERIALCOM_H
