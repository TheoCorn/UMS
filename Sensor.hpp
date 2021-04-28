
#ifndef Sensor_h
#define Sensor_h

#include <stdio.h>
#include <tuple> 
#include <vector>
#include <Wire.h>
#include <string>
#include "SPI.h"
#include <ArduinoJson.h>


class Sensor{
  public:
      Sensor(){}

      // todo: make Static
      //I wanted to use static but couldt make it work
      TwoWire &wire = Wire;
      // todo: make Static
      //I wanted to use static but couldt make it work
      static SPIClass &spiBus;

      //returns a std::vector of i2c addresses the sensor could possibly be
      virtual std::vector<uint8_t> getAddresses() = 0;


      /**
        the uuid identifies individual sensor even of the same type thus can be used for set up
        every sensor gets its uuid at runtime
        the uuid ims for internal use only, do not use uuid to change behavior of the sensor
      */

      //get the name of the Sensor
      virtual String name() = 0;
      //the actual i2c address the sensor is using
      virtual byte currentAddress() = 0;
      //default set up for setup from user use setJson()
      virtual void setUp() = 0;
      //used when getting dataFromSensor to display, so more data can be displayed
      virtual String getStringForDisplay() = 0;
      //used when reading
      //write JsonObject to the Json document
      virtual void readSensor(JsonDocument * ptrDoc) = 0;
      //get the current settings in json format
      virtual void getJson(JsonDocument * ptrDoc, uint8_t uuid) = 0;
      //set setting of the sensor
      // virtual void setJson(JsonDocument * ptrDoc) = 0;
      virtual void setJson(JsonVariant * v) = 0;

      ~Sensor() = default;
};







#endif
