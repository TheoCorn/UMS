
/*!
 *
 *  This is a library for the Adafruit BMP280 Breakout.
 *
 *  Designed specifically to work with the Adafruit BMP280 Breakout.
 *
 *  Pick one up today in the adafruit shop!
 *  ------> https://www.adafruit.com/product/2651
 *
 *  These sensors use I2C to communicate, 2 pins are required to interface.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products
 *  from Adafruit!
 *
 *  K.Townsend (Adafruit Industries)
 *
 *  BSD license, all text above must be included in any redistribution
 *
 *  original adafruit library modified by Theodor Capek
 */

#ifndef UMDWITHCLASS3_BMP280_H
#define UMDWITHCLASS3_BMP280_H

#include "Sensor.hpp"
#include "Adafruit_Sensor.h"
#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>



/*!
 *  Struct to hold calibration data.
 */
typedef struct {
    uint16_t dig_T1; /**< dig_T1 cal register. */
    int16_t dig_T2;  /**<  dig_T2 cal register. */
    int16_t dig_T3;  /**< dig_T3 cal register. */

    uint16_t dig_P1; /**< dig_P1 cal register. */
    int16_t dig_P2;  /**< dig_P2 cal register. */
    int16_t dig_P3;  /**< dig_P3 cal register. */
    int16_t dig_P4;  /**< dig_P4 cal register. */
    int16_t dig_P5;  /**< dig_P5 cal register. */
    int16_t dig_P6;  /**< dig_P6 cal register. */
    int16_t dig_P7;  /**< dig_P7 cal register. */
    int16_t dig_P8;  /**< dig_P8 cal register. */
    int16_t dig_P9;  /**< dig_P9 cal register. */
} bmp280_calib_data;

class BMP280 : public Sensor {
private:

    /*!
 * Registers available on the sensor.
 */
    enum {
        BMP280_REGISTER_DIG_T1 = 0x88,
        BMP280_REGISTER_DIG_T2 = 0x8A,
        BMP280_REGISTER_DIG_T3 = 0x8C,
        BMP280_REGISTER_DIG_P1 = 0x8E,
        BMP280_REGISTER_DIG_P2 = 0x90,
        BMP280_REGISTER_DIG_P3 = 0x92,
        BMP280_REGISTER_DIG_P4 = 0x94,
        BMP280_REGISTER_DIG_P5 = 0x96,
        BMP280_REGISTER_DIG_P6 = 0x98,
        BMP280_REGISTER_DIG_P7 = 0x9A,
        BMP280_REGISTER_DIG_P8 = 0x9C,
        BMP280_REGISTER_DIG_P9 = 0x9E,
        BMP280_REGISTER_CHIPID = 0xD0,
        BMP280_REGISTER_VERSION = 0xD1,
        BMP280_REGISTER_SOFTRESET = 0xE0,
        BMP280_REGISTER_CAL26 = 0xE1, /**< R calibration = 0xE1-0xF0 */
        BMP280_REGISTER_STATUS = 0xF3,
        BMP280_REGISTER_CONTROL = 0xF4,
        BMP280_REGISTER_CONFIG = 0xF5,
        BMP280_REGISTER_PRESSUREDATA = 0xF7,
        BMP280_REGISTER_TEMPDATA = 0xFA,
    };

    /** Oversampling rate for the sensor. */
    enum sensor_sampling {
        /** No over-sampling. */
        SAMPLING_NONE = 0x00,
        /** 1x over-sampling. */
        SAMPLING_X1 = 0x01,
        /** 2x over-sampling. */
        SAMPLING_X2 = 0x02,
        /** 4x over-sampling. */
        SAMPLING_X4 = 0x03,
        /** 8x over-sampling. */
        SAMPLING_X8 = 0x04,
        /** 16x over-sampling. */
        SAMPLING_X16 = 0x05
    };

    /** Operating mode for the sensor. */
    enum sensor_mode {
        /** Sleep mode. */
        MODE_SLEEP = 0x00,
        /** Forced mode. */
        MODE_FORCED = 0x01,
        /** Normal mode. */
        MODE_NORMAL = 0x03,
        /** Software reset. */
        MODE_SOFT_RESET_CODE = 0xB6
    };

    /** Filtering level for sensor data. */
    enum sensor_filter {
        /** No filtering. */
        FILTER_OFF = 0x00,
        /** 2x filtering. */
        FILTER_X2 = 0x01,
        /** 4x filtering. */
        FILTER_X4 = 0x02,
        /** 8x filtering. */
        FILTER_X8 = 0x03,
        /** 16x filtering. */
        FILTER_X16 = 0x04
    };

    /** Standby duration in ms */
    enum standby_duration {
        /** 1 ms standby. */
        STANDBY_MS_1 = 0x00,
        /** 62.5 ms standby. */
        STANDBY_MS_63 = 0x01,
        /** 125 ms standby. */
        STANDBY_MS_125 = 0x02,
        /** 250 ms standby. */
        STANDBY_MS_250 = 0x03,
        /** 500 ms standby. */
        STANDBY_MS_500 = 0x04,
        /** 1000 ms standby. */
        STANDBY_MS_1000 = 0x05,
        /** 2000 ms standby. */
        STANDBY_MS_2000 = 0x06,
        /** 4000 ms standby. */
        STANDBY_MS_4000 = 0x07
    };

    /** Encapsulates the config register */
    struct config {
        /** Inactive duration (standby time) in normal mode */
        unsigned int t_sb : 3;
        /** Filter settings */
        unsigned int filter : 3;
        /** Unused - don't set */
        unsigned int none : 1;
        /** Enables 3-wire SPI */
        unsigned int spi3w_en : 1;
        /** Used to retrieve the assembled config register's byte value. */
        unsigned int get() { return (t_sb << 5) | (filter << 2) | spi3w_en; }
    };

    /** Encapsulates trhe ctrl_meas register */
    struct ctrl_meas {
        /** Temperature oversampling. */
        unsigned int osrs_t : 3;
        /** Pressure oversampling. */
        unsigned int osrs_p : 3;
        /** Device mode */
        unsigned int mode : 2;
        /** Used to retrieve the assembled ctrl_meas register's byte value. */
        unsigned int get() { return (osrs_t << 5) | (osrs_p << 2) | mode; }
    };

    int32_t t_fine;
    bmp280_calib_data _bmp280_calib;
    config _configReg;
    ctrl_meas _measReg;


    static constexpr const char* jsonFilePath = "/sensorData/5.json";

    uint8_t _rsid;
    TwoWire& _wire;
    std::vector<bool> activeFeaturesVec;
    std::vector<unsigned int> xSettings;
    static constexpr const char* features_strings[2] = {"pressure", "temp"};

    void setXSettings();

    float readTemperature();
    float readPressure();
    float waterBoilingPoint(float pressure);

    void setSampling(sensor_mode mode = MODE_NORMAL,
                     sensor_sampling tempSampling = SAMPLING_X16,
                     sensor_sampling pressSampling = SAMPLING_X16,
                     sensor_filter filter = FILTER_OFF,
                     standby_duration duration = STANDBY_MS_1);

    void write8(uint8_t reg, uint8_t value);
    uint8_t read8(uint8_t reg);
    uint16_t read16(uint8_t reg);
    uint32_t read24(uint8_t reg);
    int16_t readS16(uint8_t reg);
    uint16_t read16_LE(uint8_t reg);
    int16_t readS16_LE(uint8_t reg);

public:
    BMP280(uint8_t address) : _rsid(address), _wire(Wire) {}
    ~BMP280() override = default;

    String name() override { return "BMP280"; }

    uint32_t sid() override { return sensorEnum::BMP280; }

    uint32_t rsid() override { return _rsid; }

    void setUp() override;

    String getStringForDisplay() override;

    String getExtendedStringForDisplay() override;

    void readSensor(JsonArray &jra) override;

    void saveConfig() override;

    void getJson(JsonArray &jArr) override;

    void setJson(JsonObject &sConf) override;

    float readFeature(size_t index) override;

};


#endif //UMDWITHCLASS3_BMP280_H
