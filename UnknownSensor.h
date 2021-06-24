//
// Created by theo on 6/19/2021.
//

#ifndef UMDWITHCLASS3_UNKNOWNSENSOR_H
#define UMDWITHCLASS3_UNKNOWNSENSOR_H

#include "Sensor.hpp"
#include "sensorEnum.h"

class UnknownSensor : public Sensor {
  private:
    uint8_t address;
    
  public:
    explicit UnknownSensor(uint8_t address) : address(address) {}

    String name() override { return "Unknown Sensor"; }
    byte currentAddress() override { return address; }
    void setUp() override { }
    String getStringForDisplay() override { return name(); }
    String getExtendedStringForDisplay() override { return ""; }
    void readSensor(JsonArray &jra) override { }
    void getJson(JsonDocument * ptrDoc) override {}
    void setJson(JsonVariant * v) override {}

    uint32_t sid() override { return sensorEnum::UNKNOWN_SENSOR; };
    uint32_t rsid() override { return address; }

};


#endif //UMDWITHCLASS3_UNKNOWNSENSOR_H
