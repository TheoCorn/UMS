//
// Created by theo on 6/19/2021.
//

#ifndef UMDWITHCLASS3_UNKNOWNSENSOR_H
#define UMDWITHCLASS3_UNKNOWNSENSOR_H

#include "Sensor.hpp"

class UnknownSensor : public Sensor {
    private uint8_t address;

    UnknownSensor(uint8_t address) : address(address) {}

    String name() override { return "Unknown Sensor" }
    byte currentAddress() override { return address }
    void setUp() override { }
    String getStringForDisplay() override { return name() }
    String getExtendedStringForDisplay() override { return nullptr }
    void readSensor(JsonDocument * ptrDoc) override { }
};


#endif //UMDWITHCLASS3_UNKNOWNSENSOR_H
