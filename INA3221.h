//
// Created by theodor on 11/20/2021.
//

#ifndef UMDWITHCLASS3_INA3221_H
#define UMDWITHCLASS3_INA3221_H



/**************************************************************************/
/*!
 * this code is a modified version of DL_Arduino_INA3221 Library witch is
 * derived from the INA219 adafruit library
 * @author Theodor Capek
 * @date November 2021
 *
 *   SDL_Arduino_INA3221 Library
     Version 1.2
     SwitchDoc Labs   September 2019
 *
    Initial code from INA219 code (Basically just a core structure left)
    @author   K. Townsend (Adafruit Industries)
	@license  BSD (see BSDlicense.txt)

	*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include "Sensor.hpp"
#include "sensorEnum.h"
#include <vector>
#include "Error.h"
#include "sysInfo.h"
#include "SerialCom.h"

/*=========================================================================
    CONFIG REGISTER (R/W)
    -----------------------------------------------------------------------*/
#define INA3221_REG_CONFIG                      (0x00)
/*---------------------------------------------------------------------*/
#define INA3221_CONFIG_RESET                    (0x8000)  // Reset Bit

#define INA3221_CONFIG_ENABLE_CHAN1             (0b100000000000000)  // Enable Channel 1
#define INA3221_CONFIG_ENABLE_CHAN2             (0b010000000000000)  // Enable Channel 2
#define INA3221_CONFIG_ENABLE_CHAN3             (0b001000000000000)  // Enable Channel 3


#define INA3221_CONFIG_AVG_1                    (0b000000000000)
#define INA3221_CONFIG_AVG_4                    (0b001000000000)
#define INA3221_CONFIG_AVG_16                   (0b010000000000)
#define INA3221_CONFIG_AVG_64                   (0b011000000000)
#define INA3221_CONFIG_AVG_128                  (0b100000000000)
#define INA3221_CONFIG_AVG_256                  (0b101000000000)
#define INA3221_CONFIG_AVG_512                  (0b110000000000)
#define INA3221_CONFIG_AVG_1024                 (0b111000000000)

#define INA3221_CONFIG_BUS_V_140us              (0b000000000)
#define INA3221_CONFIG_BUS_V_204us              (0b001000000)
#define INA3221_CONFIG_BUS_V_332us              (0b010000000)
#define INA3221_CONFIG_BUS_V_588us              (0b011000000)
#define INA3221_CONFIG_BUS_V_1d1ms              (0b100000000)
#define INA3221_CONFIG_BUS_V_2d1ms              (0b101000000)
#define INA3221_CONFIG_BUS_V_4d1ms              (0b110000000)
#define INA3221_CONFIG_BUS_V_8d2ms              (0b111000000)

#define INA3221_CONFIG_SHUNT_V_140us              (0b000000)
#define INA3221_CONFIG_SHUNT_V_204us              (0b001000)
#define INA3221_CONFIG_SHUNT_V_332us              (0b010000)
#define INA3221_CONFIG_SHUNT_V_588us              (0b011000)
#define INA3221_CONFIG_SHUNT_V_1d1ms              (0b100000)
#define INA3221_CONFIG_SHUNT_V_2d1ms              (0b101000)
#define INA3221_CONFIG_SHUNT_V_4d1ms              (0b110000)
#define INA3221_CONFIG_SHUNT_V_8d2ms              (0b111000)

#define INA3221_CONFIG_MODE_CONTINUOUS          (0b111)


//#define INA3221_CONFIG_AVG2                     (0x0800)  // AVG Samples Bit 2 - See table 3 spec
//#define INA3221_CONFIG_AVG1                     (0x0400)  // AVG Samples Bit 1 - See table 3 spec
//#define INA3221_CONFIG_AVG0                     (0x0200)  // AVG Samples Bit 0 - See table 3 spec

//#define INA3221_CONFIG_VBUS_CT2                 (0x0100)  // VBUS bit 2 Conversion time - See table 4 spec
//#define INA3221_CONFIG_VBUS_CT1                 (0x0080)  // VBUS bit 1 Conversion time - See table 4 spec
//#define INA3221_CONFIG_VBUS_CT0                 (0x0040)  // VBUS bit 0 Conversion time - See table 4 spec

//#define INA3221_CONFIG_VSH_CT2                  (0x0020)  // Vshunt bit 2 Conversion time - See table 5 spec
//#define INA3221_CONFIG_VSH_CT1                  (0x0010)  // Vshunt bit 1 Conversion time - See table 5 spec
//#define INA3221_CONFIG_VSH_CT0                  (0x0008)  // Vshunt bit 0 Conversion time - See table 5 spec



/*=========================================================================*/

/*=========================================================================
    SHUNT VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
#define INA3221_REG_SHUNTVOLTAGE_1                (0x01)
/*=========================================================================*/

/*=========================================================================
    BUS VOLTAGE REGISTER (R)
    -----------------------------------------------------------------------*/
#define INA3221_REG_BUSVOLTAGE_1                  (0x02)
/*=========================================================================*/

#define SHUNT_RESISTOR_VALUE  (0.1)   // default shunt resistor value of 0.1 Ohm

/**
 * @addtogroup sensors
 * @{
 */

class INA3221 : public Sensor {
private:
    static constexpr const char* jsonFilePath = "/sensorData/4.json";

    uint8_t _rsid;

    static constexpr char const* inaFeaturesString[9] = {"busV1",
                                                         "shuntmV1",
                                                         "current1",
                                                         "busV2",
                                                         "shuntmV2",
                                                         "current2",
                                                         "busV3",
                                                         "shuntmV3",
                                                         "current3"};
    std::vector<bool> activeFeaturesVec;
    std::vector<unsigned int> xSettings;
    TwoWire& wire;


    float _shunt_resistor;

    float getBusVoltage_V(int channel);
    float getShuntVoltage_mV(int channel);
    float getCurrent_mA(int channel);
    int getManufID();

    void wireWriteRegister(uint8_t reg, uint16_t value);
    void wireReadRegister(uint8_t reg, uint16_t *value);
    void INA3221SetConfig();
    int16_t getBusVoltage_raw(int channel);
    int16_t getShuntVoltage_raw(int channel);


public:
//    INA3221(uint32_t _rsid, TwoWire& wire) : _rsid(_rsid), wire(wire) {}

    explicit INA3221(uint8_t rsid) : _rsid(rsid), wire(Wire) {
//        wire = Wire;
}



    uint32_t rsid() override { return _rsid; }
    uint32_t sid() override { return sensorEnum::INA3221; }

    String name() override;

    void setUp() override;

    String getStringForDisplay() override;

    String getExtendedStringForDisplay() override;

    void readSensor(JsonArray &jra) override;

    void saveConfig() override;

    void getJson(JsonArray &jArr) override;

    void setJson(JsonObject &sConf) override;

    float readFeature(size_t index) override;

};

/** @} */


#endif //UMDWITHCLASS3_INA3221_H
