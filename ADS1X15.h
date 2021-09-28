//
// Created by theo on 9/27/2021.
//

#ifndef UMDWITHCLASS3_ADS1X15_H
#define UMDWITHCLASS3_ADS1X15_H

#include "Sensor.hpp"
#include <Adafruit_I2CDevice.h>
#include <Arduino.h>
#include <Wire.h>
#include "sensorEnum.h"

#include "SPIFFS.h"
#include "Error.h"
#include "spiffs.hpp"


/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define ADS1X15_ADDRESS (0x48) ///< 1001 000 (ADDR = GND)
/*=========================================================================*/

/*=========================================================================
    POINTER REGISTER
    -----------------------------------------------------------------------*/
#define ADS1X15_REG_POINTER_MASK (0x03)      ///< Point mask
#define ADS1X15_REG_POINTER_CONVERT (0x00)   ///< Conversion
#define ADS1X15_REG_POINTER_CONFIG (0x01)    ///< Configuration
#define ADS1X15_REG_POINTER_LOWTHRESH (0x02) ///< Low threshold
#define ADS1X15_REG_POINTER_HITHRESH (0x03)  ///< High threshold
/*=========================================================================*/

/*=========================================================================
    CONFIG REGISTER
    -----------------------------------------------------------------------*/
#define ADS1X15_REG_CONFIG_OS_MASK (0x8000) ///< OS Mask
#define ADS1X15_REG_CONFIG_OS_SINGLE                                           \
  (0x8000) ///< Write: Set to start a single-conversion
#define ADS1X15_REG_CONFIG_OS_BUSY                                             \
  (0x0000) ///< Read: Bit = 0 when conversion is in progress
#define ADS1X15_REG_CONFIG_OS_NOTBUSY                                          \
  (0x8000) ///< Read: Bit = 1 when device is not performing a conversion

#define ADS1X15_REG_CONFIG_MUX_MASK (0x7000) ///< Mux Mask
#define ADS1X15_REG_CONFIG_MUX_DIFF_0_1                                        \
  (0x0000) ///< Differential P = AIN0, N = AIN1 (default)
#define ADS1X15_REG_CONFIG_MUX_DIFF_0_3                                        \
  (0x1000) ///< Differential P = AIN0, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_DIFF_1_3                                        \
  (0x2000) ///< Differential P = AIN1, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_DIFF_2_3                                        \
  (0x3000) ///< Differential P = AIN2, N = AIN3
#define ADS1X15_REG_CONFIG_MUX_SINGLE_0 (0x4000) ///< Single-ended AIN0
#define ADS1X15_REG_CONFIG_MUX_SINGLE_1 (0x5000) ///< Single-ended AIN1
#define ADS1X15_REG_CONFIG_MUX_SINGLE_2 (0x6000) ///< Single-ended AIN2
#define ADS1X15_REG_CONFIG_MUX_SINGLE_3 (0x7000) ///< Single-ended AIN3

#define ADS1X15_REG_CONFIG_PGA_MASK (0x0E00)   ///< PGA Mask
#define ADS1X15_REG_CONFIG_PGA_6_144V (0x0000) ///< +/-6.144V range = Gain 2/3
#define ADS1X15_REG_CONFIG_PGA_4_096V (0x0200) ///< +/-4.096V range = Gain 1
#define ADS1X15_REG_CONFIG_PGA_2_048V                                          \
  (0x0400) ///< +/-2.048V range = Gain 2 (default)
#define ADS1X15_REG_CONFIG_PGA_1_024V (0x0600) ///< +/-1.024V range = Gain 4
#define ADS1X15_REG_CONFIG_PGA_0_512V (0x0800) ///< +/-0.512V range = Gain 8
#define ADS1X15_REG_CONFIG_PGA_0_256V (0x0A00) ///< +/-0.256V range = Gain 16

#define ADS1X15_REG_CONFIG_MODE_MASK (0x0100)   ///< Mode Mask
#define ADS1X15_REG_CONFIG_MODE_CONTIN (0x0000) ///< Continuous conversion mode
#define ADS1X15_REG_CONFIG_MODE_SINGLE                                         \
  (0x0100) ///< Power-down single-shot mode (default)

#define ADS1X15_REG_CONFIG_RATE_MASK (0x00E0) ///< Data Rate Mask

#define ADS1X15_REG_CONFIG_CMODE_MASK (0x0010) ///< CMode Mask
#define ADS1X15_REG_CONFIG_CMODE_TRAD                                          \
  (0x0000) ///< Traditional comparator with hysteresis (default)
#define ADS1X15_REG_CONFIG_CMODE_WINDOW (0x0010) ///< Window comparator

#define ADS1X15_REG_CONFIG_CPOL_MASK (0x0008) ///< CPol Mask
#define ADS1X15_REG_CONFIG_CPOL_ACTVLOW                                        \
  (0x0000) ///< ALERT/RDY pin is low when active (default)
#define ADS1X15_REG_CONFIG_CPOL_ACTVHI                                         \
  (0x0008) ///< ALERT/RDY pin is high when active

#define ADS1X15_REG_CONFIG_CLAT_MASK                                           \
  (0x0004) ///< Determines if ALERT/RDY pin latches once asserted
#define ADS1X15_REG_CONFIG_CLAT_NONLAT                                         \
  (0x0000) ///< Non-latching comparator (default)
#define ADS1X15_REG_CONFIG_CLAT_LATCH (0x0004) ///< Latching comparator

#define ADS1X15_REG_CONFIG_CQUE_MASK (0x0003) ///< CQue Mask
#define ADS1X15_REG_CONFIG_CQUE_1CONV                                          \
  (0x0000) ///< Assert ALERT/RDY after one conversions
#define ADS1X15_REG_CONFIG_CQUE_2CONV                                          \
  (0x0001) ///< Assert ALERT/RDY after two conversions
#define ADS1X15_REG_CONFIG_CQUE_4CONV                                          \
  (0x0002) ///< Assert ALERT/RDY after four conversions
#define ADS1X15_REG_CONFIG_CQUE_NONE                                           \
  (0x0003) ///< Disable the comparator and put ALERT/RDY in high state (default)
/*=========================================================================*/

/** Gain settings */
typedef enum {
    GAIN_TWOTHIRDS = ADS1X15_REG_CONFIG_PGA_6_144V,
    GAIN_ONE = ADS1X15_REG_CONFIG_PGA_4_096V,
    GAIN_TWO = ADS1X15_REG_CONFIG_PGA_2_048V,
    GAIN_FOUR = ADS1X15_REG_CONFIG_PGA_1_024V,
    GAIN_EIGHT = ADS1X15_REG_CONFIG_PGA_0_512V,
    GAIN_SIXTEEN = ADS1X15_REG_CONFIG_PGA_0_256V
} adsGain_t;

/** Data rates */
#define RATE_ADS1015_128SPS (0x0000)  ///< 128 samples per second
#define RATE_ADS1015_250SPS (0x0020)  ///< 250 samples per second
#define RATE_ADS1015_490SPS (0x0040)  ///< 490 samples per second
#define RATE_ADS1015_920SPS (0x0060)  ///< 920 samples per second
#define RATE_ADS1015_1600SPS (0x0080) ///< 1600 samples per second (default)
#define RATE_ADS1015_2400SPS (0x00A0) ///< 2400 samples per second
#define RATE_ADS1015_3300SPS (0x00C0) ///< 3300 samples per second

#define RATE_ADS1115_8SPS (0x0000)   ///< 8 samples per second
#define RATE_ADS1115_16SPS (0x0020)  ///< 16 samples per second
#define RATE_ADS1115_32SPS (0x0040)  ///< 32 samples per second
#define RATE_ADS1115_64SPS (0x0060)  ///< 64 samples per second
#define RATE_ADS1115_128SPS (0x0080) ///< 128 samples per second (default)
#define RATE_ADS1115_250SPS (0x00A0) ///< 250 samples per second
#define RATE_ADS1115_475SPS (0x00C0) ///< 475 samples per second
#define RATE_ADS1115_860SPS (0x00E0) ///< 860 samples per second

class ADS1X15 {
protected:
    // Instance-specific properties
    Adafruit_I2CDevice *m_i2c_dev; ///< I2C bus device
    uint8_t m_bitShift;            ///< bit shift amount
    adsGain_t m_gain;              ///< ADC gain
    uint16_t m_dataRate;           ///< Data rate

    void getJson(JsonArray& jArr, uint32_t rsid, uint32_t sid);

    void setJson(JsonObject& sConf);

    void setUp(const char *filename);

    void readSensor(JsonArray &jra, uint32_t rsid, Sensor* sensor);

    float readFeature(size_t index);

    String getExtendedStringForDisplay(Sensor *sensor);

    void saveConfig();

public:


    bool begin(uint8_t i2c_addr = ADS1X15_ADDRESS, TwoWire *wire = &Wire);
    int16_t readADC_SingleEnded(uint8_t channel);
    int16_t readADC_Differential_0_1();
    int16_t readADC_Differential_2_3();
    void startComparator_SingleEnded(uint8_t channel, int16_t threshold);
    int16_t getLastConversionResults();
    float computeVolts(int16_t counts);
    void setGain(adsGain_t gain);
    adsGain_t getGain();
    void setDataRate(uint16_t rate);
    uint16_t getDataRate();

private:
    char const* adsFeaturesString[3] = {"SEV0", "SEV1", "DIFV"};
    std::vector<bool> activeFeaturesVec;
    std::vector<unsigned int> xSettings;



    bool conversionComplete();
    void writeRegister(uint8_t reg, uint16_t value);
    uint16_t readRegister(uint8_t reg);
    uint8_t buffer[3];

};

class ADS1015 : public ADS1X15, public Sensor{
public:
    ADS1015(uint32_t address);

    String name() override { return "ADS1015"; }

    uint32_t sid() override { return sensorEnum::ADS1015; }
    uint32_t rsid() override { return m_i2c_dev->address(); }

    void getJson(JsonArray& jArr) override { return ADS1X15::getJson(jArr, rsid(), sid()); }

    void setJson(JsonObject& sConf) override { ADS1X15::setJson(sConf); }

    void setUp() override { ADS1X15::setUp("/sensorData/2.json"); }

    void readSensor(JsonArray& jra) override { ADS1X15::readSensor(jra, rsid(), dynamic_cast<Sensor*>(this)); }

    float readFeature(size_t index) override { return ADS1X15::readFeature(index); }

    String getStringForDisplay() override { return name(); }

//    char const * getExtendedStringForDisplay() override;
    String getExtendedStringForDisplay() override { return ADS1X15::getExtendedStringForDisplay(this); }

    void saveConfig() override { ADS1X15::saveConfig(); }
};

/**************************************************************************/
/*!
    @brief  Sensor driver for the Adafruit ADS1115 ADC breakout.
*/
/**************************************************************************/
class ADS1115 : public ADS1X15, public Sensor {
public:
    ADS1115(uint32_t address);

    String name() override { return "ADS1115"; }

    uint32_t sid() override { return sensorEnum::ADS1015; }
    uint32_t rsid() override { return m_i2c_dev->address(); }

    void getJson(JsonArray& jArr) override { return ADS1X15::getJson(jArr); }

    void setJson(JsonObject& sConf) override { ADS1X15::setJson(sConf); }

    void setUp() override { ADS1X15::setUp("/sensorData/3.json"); }

    void readSensor(JsonArray& jra) override { ADS1X15::readSensor(jra); }

    float readFeature(size_t index) override { return ADS1X15::readFeature(index); }

    String getStringForDisplay() override { return name(); }

//    char const * getExtendedStringForDisplay() override;
    String getExtendedStringForDisplay() override { return getExtendedStringForDisplay(this); }

    void saveConfig() override { ADS1X15::saveConfig(); }
};


#endif //UMDWITHCLASS3_ADS1X15_H
