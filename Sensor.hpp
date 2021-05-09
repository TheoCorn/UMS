
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
      Sensor()= default;


      /**
        the uuid identifies individual sensor even of the same type thus can be used for set up
        every sensor gets its uuid at runtime
        the uuid ims for internal use only, do not use uuid to change behavior of the sensor

        kinda wired but OK me
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
      /*
       * get the current settings in json format
       *
       * the method has to create a nested object in the JsonDocument which name should reflect the sensor name
       * (from now known as the sensor object)
       * remember that multiple sensors of the same function can be present
       *
       * the method has to create a variable in the sensor object called uuid witch is the i2c address
       *
       * the method has to create a nested object in the sensor object called "features"
       * each variable in features represents a dataset for the graph (one feature = one line in graph)
       * "features"{
       *    "feature0" = isUsed<Bool>,
       *    "feature1" = isUsed<Bool>,
       *        ...
       *   }
       *
       * the method can create a nested object in the sensor object called "XSettings" short for exclusive settings
       * basically forces the user to chose between multiple options
       * meant to be used for stuff like accuracy / range settings
       *
       * @param JsonDocument*
       */
      virtual void getJson(JsonDocument * ptrDoc) = 0;
      //set setting of the sensor
      // virtual void setJson(JsonDocument * ptrDoc) = 0;
      virtual void setJson(JsonVariant * v) = 0;

      virtual ~Sensor() = default;
};







#endif
