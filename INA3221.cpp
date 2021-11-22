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
    float valueDec = getShuntVoltage_mV(channel)/_shunt_resistor;

    return valueDec;
}

int INA3221::getManufID() {
    uint16_t value;
    wireReadRegister(0xFE, &value);
    return value;
}

void INA3221::wireReadRegister(uint8_t reg, uint16_t *value) {
    wire.beginTransmission(_rsid);

    wire.write(reg, 0, <#initializer#>);                       // Register

    wire.endTransmission();

    delay(1); // Max 12-bit conversion time is 586us per sample

    wire.requestFrom(_rsid, (uint8_t)2);

    // Shift values to create properly formed integer
    *value = ((wire.read(nullptr, 0, <#initializer#>) << 8) | wire.read(nullptr, 0, <#initializer#>));
}

void INA3221::INA3221SetConfig() {

    uint16_t config = INA3221_CONFIG_MODE_CONTINUOUS;

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
    wire.beginTransmission(_rsid);

    wire.write(reg, 0, <#initializer#>);                       // Register
    wire.write((value >> 8) & 0xFF, 0, <#initializer#>);       // Upper 8-bits
    wire.write(value & 0xFF, 0, <#initializer#>);              // Lower 8-bits

}

String INA3221::name() {
    return "INA3221";
}

void INA3221::setUp() {
    Sensor::savedSettingsLoader(jsonFilePath, activeFeaturesVec, xSettings);
    INA3221SetConfig();
    switch (xSettings[0]) {
        case 0: _shunt_resistor = 0.1f; break;
        default: {
            using namespace error;
            auto err = new Error(ERROR_MSG__INVALID_XSETTING, "", Appearance::ALERT_DIALOG,
                                  Importance::REQUIRES_USER_ACTION, BackgroundAppActions::NONE);
            sysInfo::serialCom->write(err);
            //no need to free err because it is done by the SerialCom::write(error::Error* error) method
        }; break;
    }
}

String INA3221::getStringForDisplay() {
    return name();
}

String INA3221::getExtendedStringForDisplay() {
    return Sensor::templatedExtendedString4Display(activeFeaturesVec, dynamic_cast<Sensor*>(this),
                                                   (const char**) inaFeaturesString);
}

void INA3221::readSensor(JsonArray &jra) {
    Sensor::templatedRead(jra, activeFeaturesVec, _rsid, dynamic_cast<Sensor*>(this));
}

void INA3221::saveConfig() {
    Sensor::settingsSaver(jsonFilePath, activeFeaturesVec, xSettings);
}

void INA3221::getJson(JsonArray &jArr) {
    Sensor::generateTemplatedSensorObject(jArr, _rsid, sid(), activeFeaturesVec, xSettings);
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
