//
// Created by theo on 6/19/2021.
//

#ifndef UMDWITHCLASS3_UNKNOWNSENSOR_H
#define UMDWITHCLASS3_UNKNOWNSENSOR_H

#include "Sensor.hpp"

class UnknownSensor : public Sensor {
  private:
    uint8_t address;
    
  public:
    UnknownSensor(uint8_t address) : address(address) {}

    String name() override { return "Unknown Sensor"; }
    byte currentAddress() override { return address; }
    void setUp() override { }
    String getStringForDisplay() override { return name(); }
    String getExtendedStringForDisplay() override { return ""; }
    void readSensor(JsonArray &jra) override { }
    void getJson(JsonDocument * ptrDoc) override {}
    void setJson(JsonVariant * v) override {}

    uint32_t sid() override { return 0; };
    uint32_t rsid() override { return address; }

};


#endif //UMDWITHCLASS3_UNKNOWNSENSOR_H
