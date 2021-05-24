//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_UART_H
#define UMDWITHCLASS3_UART_H

#include "SerialCom.h"
#include <ArduinoJson.h>
#include "asciiMakros.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


class Uart: public SerialCom {

public:

    Uart();
    ~Uart();

    bool begin() override;
    int available() override;
    void read(char* c) override;
    size_t write(const uint8_t) override;
    size_t write(const uint8_t *buffer, size_t size) override;
    size_t write(JsonDocument* doc) override;
    void flush() override;

    void startConnectionCheck(int duration) override;

    bool hasConnectedDevice() override;

    char* getString4Display() override {return nullptr;};

    TaskHandle_t UartConnCheckHandle = NULL;


private:
    bool connected = false;
//    Serial * serial;

protected:


    static void connectionCheckTask(void* connInfo);
    static void readTask(void* connected);
};


#endif //UMDWITHCLASS3_UART_H
