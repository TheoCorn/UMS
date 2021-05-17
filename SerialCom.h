//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_SERIALCOM_H
#define UMDWITHCLASS3_SERIALCOM_H

#include <ArduinoJson.h>
#include "JsonSerializer.h"


//class to pass to the task startConnectionCheck usually calls
class ConnCheckInfo{
public:

    /*
     * @param dur
     * @param conn
     */
    ConnCheckInfo(int dur, bool* conn){
        this->duration = dur;
        this->connected = conn;
    }

    int duration;
    bool* connected;
};

class SerialCom {
public:

    SerialCom();
    ~SerialCom();

    /*
     * setups the serial port
     *
     * @return boolean port started successfully
     */
    virtual bool begin() = 0;

    /*
     * @return number of bytes available
     */
    virtual int available() = 0;

    /*
     * gets the first byte and stores it
     *
     * @param c pointer to char where to store the byte
     */
    virtual void read(char* c) = 0;

    /*
     * @param write the byte to write
     */
    virtual size_t write(const uint8_t c) = 0;

    /*
     * writes a uint8_t array to the serial port
     *
     * @param buffer
     * @param size
     */
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;

    /*
     * serializes a JsonDocument and sends it to the serial port
     *
     * @param doc
     */
    virtual size_t write(JsonDocument * doc) = 0;

    /*
     * flushes the Rx buffer
     */
    virtual void flush();

    /*
     * usually starts a freeRTOS task that periodically sends STX ascii char
     *
     */
    virtual void startConnectionCheck(int duration) = 0;
    virtual bool hasConnectedDevice() = 0;

};


#endif //UMDWITHCLASS3_SERIALCOM_H
