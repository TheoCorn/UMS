//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_SERIALCOM_H
#define UMDWITHCLASS3_SERIALCOM_H

#include <ArduinoJson.h>
#include "JsonSerializer.h"
#include <vector>
#include "ConflictingSensors.hpp"
#include <string>



//class to pass to the task startConnectionCheck usually calls
class ConnCheckInfo{
public:

    int duration;
    bool* connected;
    TaskHandle_t* taskHandle;

    /***
     * @param dur duration in millis
     * @param conn pointer to boolean to be changed
     * @param taskHandle pointer to task handle so the task can delete itself
     */
    ConnCheckInfo(int dur, bool* conn, TaskHandle_t* taskHandle) : duration(dur), connected(conn), taskHandle(taskHandle){}


};

/**
 * @defgroup com communication
 */
class SerialCom {
public:

    SerialCom() = default;
    virtual ~SerialCom() = default;

    /***
     * setups the serial port
     *
     * @return boolean port started successfully
     */
    virtual bool begin() = 0;

    /***
     * @return number of bytes available
     */
    virtual std::size_t available() = 0;

    /***
     * gets the first byte and stores it
     *
     * @param c pointer to char where to store the byte
     * @internal
     * do not use
     */
    virtual void read(char* c) = 0;

    /***
     * @param c the byte to write
     * @internal
     * do not use
     */
    virtual size_t write(uint8_t c) = 0;

    /**
     * writes a uint8_t array to the serial port
     *
     * @param buffer
     * @param size
     */
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;

    /**
     * serializes a JsonDocument and sends it to the serial port
     *
     * @param doc
     */
    virtual size_t write(JsonDocument * doc) = 0;

    /**
     * Serializes an Error as Json, sends it and frees the Error from memory
     *
     * @param error the error to send
     */
    virtual size_t write(error::Error* error) = 0;

    /**
     * flushes the Rx buffer
     */
    virtual void flush() = 0;

    /**
     * usually starts a freeRTOS task that periodically sends STX ascii char depends on implementation
     * can also do completely nothing like in bluetooth SPP
     *
     * @param duration time in millis to check weather a device is connected
     */
    virtual void startConnectionCheck(int duration) = 0;

    /**
     * @return true if at least one device is connected
     */
    virtual bool hasConnectedDevice() = 0;

    /**
     *@return some interesting information about the protocol state could be an ip/mac address... or error...
     * nullptr is a valid output
     */
    virtual char* getString4Display() = 0;

};

namespace sce{
#define SERIAL_COMM_NAMES {"Bluetooth SPP", "USB serial", "MQTT"}
    enum SerialComs: unsigned int{
        BLUETOOTH_SPP   = 0,
        USB_SERIAL      = 1,
        MQTT            = 2
    };
}

SerialCom* getSerialCom4EnumPos(unsigned int enumPos);



#endif //UMDWITHCLASS3_SERIALCOM_H
