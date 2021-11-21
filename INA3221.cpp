//
// Created by theo on 11/20/2021.
//

#include "INA3221.h"


float INA3221::getBusVoltage_V(int channel) {
    int16_t value = getBusVoltage_raw(channel);
    return value * 0.001f;
}

float INA3221::getShuntVoltage_mV(int channel) {
    int16_t value;
    value = getShuntVoltage_raw(channel);
    return value * 0.005f;
}

float INA3221::getCurrent_mA(int channel) {
    float valueDec = getShuntVoltage_mV(channel)/INA3221_shuntresistor;

    return valueDec;
}

int INA3221::getManufID() {
    uint16_t value;
    wireReadRegister(0xFE, &value);
    return value;
}

void INA3221::wireReadRegister(uint8_t reg, uint16_t *value) {
    Wire.beginTransmission(rsid);
#if ARDUINO >= 100
    Wire.write(reg);                       // Register
#else
    Wire.send(reg);                        // Register
#endif
    Wire.endTransmission();

    delay(1); // Max 12-bit conversion time is 586us per sample

    Wire.requestFrom(rsid, (uint8_t)2);
#if ARDUINO >= 100
    // Shift values to create properly formed integer
    *value = ((Wire.read() << 8) | Wire.read());
#else
    // Shift values to create properly formed integer
    *value = ((Wire.receive() << 8) | Wire.receive());
#endif
}

void INA3221::INA3221SetConfig() {

    uint16_t config = 0b0;

    if (activeFeaturesVec[0] || activeFeaturesVec[1] || activeFeaturesVec[2]) config | INA3221_CONFIG_ENABLE_CHAN1;
    if (activeFeaturesVec[3] || activeFeaturesVec[4] || activeFeaturesVec[5]) config | INA3221_CONFIG_ENABLE_CHAN2;
    if (activeFeaturesVec[6] || activeFeaturesVec[7] || activeFeaturesVec[8]) config | INA3221_CONFIG_ENABLE_CHAN3;

    switch (xSettings[1]) {
        case 0: config | INA3221_CONFIG_AVG_1; break;
        case 1: config | INA3221_CONFIG_AVG_4; break;
        case 2: config | INA3221_CONFIG_AVG_16; break;
        case 3: config | INA3221_CONFIG_AVG_64; break;
        case 4: config | INA3221_CONFIG_AVG_128; break;
        case 5: config | INA3221_CONFIG_AVG_256; break;
        case 6: config | INA3221_CONFIG_AVG_512; break;
        case 7: config | INA3221_CONFIG_AVG_1024; break;
    }

    switch (xSettings[2]) {
        case 0: config | INA3221_CONFIG_BUS_V_140us; break;
        case 1: config | INA3221_CONFIG_BUS_V_204us; break;
        case 2: config | INA3221_CONFIG_BUS_V_332us; break;
        case 3: config | INA3221_CONFIG_BUS_V_588us; break;
        case 4: config | INA3221_CONFIG_BUS_V_1d1ms; break;
        case 5: config | INA3221_CONFIG_BUS_V_2d1ms; break;
        case 6: config | INA3221_CONFIG_BUS_V_4d1ms; break;
        case 7: config | INA3221_CONFIG_BUS_V_8d2ms; break;
    }

    switch (xSettings[3]) {
        case 0: config | INA3221_CONFIG_SHUNT_V_140us; break;
        case 1: config | INA3221_CONFIG_SHUNT_V_204us; break;
        case 2: config | INA3221_CONFIG_SHUNT_V_332us; break;
        case 3: config | INA3221_CONFIG_SHUNT_V_588us; break;
        case 4: config | INA3221_CONFIG_SHUNT_V_1d1ms; break;
        case 5: config | INA3221_CONFIG_SHUNT_V_2d1ms; break;
        case 6: config | INA3221_CONFIG_SHUNT_V_4d1ms; break;
        case 7: config | INA3221_CONFIG_SHUNT_V_8d2ms; break;
    }

    wireWriteRegister(INA3221_REG_CONFIG, config);
}

int16_t INA3221::getBusVoltage_raw(int channel) {
    uint16_t value;
    wireReadRegister(INA3221_REG_BUSVOLTAGE_1+(channel) *2, &value);
//    Serial.print("BusVoltage_raw=");
//    Serial.println(value,HEX);

    // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
    return (int16_t)(value );
}

int16_t INA3221::getShuntVoltage_raw(int channel) {
    uint16_t value;
    wireReadRegister(INA3221_REG_SHUNTVOLTAGE_1+(channel) *2, &value);
    // Serial.print("ShuntVoltage_raw=");
    // Serial.println(value,HEX);
    return (int16_t)value;
}

void INA3221::wireWriteRegister(uint8_t reg, uint16_t value) {
    Wire.beginTransmission(rsid);
#if ARDUINO >= 100
    Wire.write(reg);                       // Register
    Wire.write((value >> 8) & 0xFF);       // Upper 8-bits
    Wire.write(value & 0xFF);              // Lower 8-bits
    Wire.send(reg);                        // Register
    Wire.send(value >> 8);                 // Upper 8-bits
    Wire.send(value & 0xFF);               // Lower 8-bits
#endif
    Wire.endTransmission();
}

String INA3221::name() {
    return "INA3221";
}

void INA3221::setUp() {
    Sensor::savedSettingsLoader("/sensorData/4.json", activeFeaturesVec, xSettings);
    INA3221SetConfig();
}

String INA3221::getStringForDisplay() {
    return name();
}

String INA3221::getExtendedStringForDisplay() {
    return Sensor::templatedExtendedString4Display(activeFeaturesVec, dynamic_cast<Sensor*>(this),
                                                   (const char**) inaFeaturesString);
}

void INA3221::readSensor(JsonArray &jra) {
    Sensor::templatedRead(jra, activeFeaturesVec, rsid, dynamic_cast<Sensor*>(this));
}

void INA3221::saveConfig() {

}

void INA3221::getJson(JsonArray &jArr) {
    Sensor::generateTemplatedSensorObject(jArr, rsid, sid(), activeFeaturesVec, xSettings);
}

void INA3221::setJson(JsonObject &sConf) {
    Sensor::JsonSetter(sConf, activeFeaturesVec, xSettings);
    INA3221SetConfig();
}

float INA3221::readFeature(size_t index) {
    switch (index) {
        case 0 : return getBusVoltage_V(0);
        case 1 : return getShuntVoltage_mV(0);
        case 2 : return getCurrent_mA(0);
        case 3 : return getBusVoltage_V(1);
        case 4 : return getShuntVoltage_mV(1);
        case 5 : return getCurrent_mA(1);
        case 6 : return getBusVoltage_V(2);
        case 7 : return getShuntVoltage_mV(2);
        case 8 : return getCurrent_mA(2);
    }
}

