//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_UART_H
#define UMDWITHCLASS3_UART_H

#include "SerialCom.h"
#include <ArduinoJson>


class Uart: public SerialCom {

public:

    Uart();
    ~Uart()

    bool begin() override;
    int available() override;
    bool read(char* c) override;
    size_t write(const uint8_t) override;
    size_t write(const uint8_t *buffer, size_t) override;
    size_t write(const JsonDocument * doc) override;
    void flush() override;

private:
//    Serial * serial;

};


#endif //UMDWITHCLASS3_UART_H
