//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_UART_H
#define UMDWITHCLASS3_UART_H

#include "SerialCom.h"
#include <ArduinoJson.h>
#include "asciiMakros.h"


class Uart: public SerialCom {

public:

    Uart();
    ~Uart();

    bool begin() override;
    int available() override;
    bool read(char* c) override;
    size_t write(const uint8_t) override;
    size_t write(const uint8_t *buffer, size_t size) override;
    size_t write(JsonDocument * doc) override;
    void flush() override;

    void startConnectionCheck(int duration) override;
    void connectionCheckTask(void* duration);
    bool hasConnectedDevice() override;


private:
    bool connected = false;
//    Serial * serial;

protected:
    TaskHandle_t UartSenderHandle = NULL;
    TaskHandle_t UartReaderTask = NULL;

    static void connectionCheckTask();
    static void readTask(bool* connected);
};


#endif //UMDWITHCLASS3_UART_H
