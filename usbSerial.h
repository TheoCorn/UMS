//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_USBSERIAL_H
#define UMDWITHCLASS3_USBSERIAL_H

#include "SerialCom.h"
#include <ArduinoJson.h>
#include "asciiMakros.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Error.h"
#include "asciiMakros.h"


class usbSerial: public SerialCom {

public:

    usbSerial();
    ~usbSerial();

    bool begin() override;
    std::size_t available() override;
    void read(char* c) override;
    size_t write(const uint8_t) override;
    size_t write(const uint8_t *buffer, size_t size) override;
    size_t write(JsonDocument* doc) override;
    size_t write(error::Error* error) override;
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


#endif //UMDWITHCLASS3_USBSERIAL_H
