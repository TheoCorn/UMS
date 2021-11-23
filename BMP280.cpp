//
// Created by theo on 5/9/2021.
//

#include "BMP280.h"



void BMP280::setUp() {
    Sensor::savedSettingsLoader(jsonFilePath, activeFeaturesVec, xSettings);
    setXSettings();
}

String BMP280::getStringForDisplay() {
    return name();
}

String BMP280::getExtendedStringForDisplay() {
//    Serial.println(activeFeaturesVec.size());

    return Sensor::templatedExtendedString4Display(activeFeaturesVec,
                                                   dynamic_cast<Sensor*>(this), features_strings);
}

void BMP280::readSensor(JsonArray &jra) {
    Sensor::templatedRead(jra, activeFeaturesVec, rsid(), dynamic_cast<Sensor*>(this));
}

void BMP280::saveConfig() {
    Sensor::settingsSaver(jsonFilePath, activeFeaturesVec, xSettings);
}

void BMP280::getJson(JsonArray &jArr) {
    Sensor::generateTemplatedSensorObject(jArr, _rsid, sid(), activeFeaturesVec, xSettings);
}

void BMP280::setJson(JsonObject &sConf) {
    Sensor::JsonSetter(sConf, activeFeaturesVec, xSettings);
    setXSettings();
}

float BMP280::readFeature(size_t index) {
    switch (index) {
        case 0: return readPressure();
        case 1: return readTemperature();
        case 2: return waterBoilingPoint(readPressure());
    }
}


/*!
 * Sets the sampling config for the device.
 * @param mode
 *        The operating mode of the sensor.
 * @param tempSampling
 *        The sampling scheme for temp readings.
 * @param pressSampling
 *        The sampling scheme for pressure readings.
 * @param filter
 *        The filtering mode to apply (if any).
 * @param duration
 *        The sampling duration.
 */
void BMP280::setSampling(sensor_mode mode,
                                  sensor_sampling tempSampling,
                                  sensor_sampling pressSampling,
                                  sensor_filter filter,
                                  standby_duration duration) {
    _measReg.mode = mode;
    _measReg.osrs_t = tempSampling;
    _measReg.osrs_p = pressSampling;

    _configReg.filter = filter;
    _configReg.t_sb = duration;

    write8(BMP280_REGISTER_CONFIG, _configReg.get());
    write8(BMP280_REGISTER_CONTROL, _measReg.get());
}

/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C/SPI
*/
/**************************************************************************/
void BMP280::write8(uint8_t reg, uint8_t value) {
    uint8_t buffer[2];
    buffer[1] = value;

        buffer[0] = reg;
    Sensor::write(_rsid, buffer, 2, _wire);
}

/*!
 *  @brief  Reads an 8 bit value over I2C/SPI
 *  @param  reg
 *          selected register
 *  @return value from selected register
 */
uint8_t BMP280::read8(uint8_t reg) {
    uint8_t buffer[1];
        buffer[0] = uint8_t(reg);
    Sensor::write_then_read(_rsid, buffer, 1, buffer, 1, _wire);
    return buffer[0];
}

/*!
 *  @brief  Reads a 16 bit value over I2C/SPI
 */
uint16_t BMP280::read16(uint8_t reg) {
    uint8_t buffer[2];

        buffer[0] = uint8_t(reg);
    Sensor::write_then_read(_rsid, buffer, 1, buffer, 2, _wire);
    return uint16_t(buffer[0]) << 8 | uint16_t(buffer[1]);
}

uint16_t BMP280::read16_LE(uint8_t reg) {
    uint16_t temp = read16(reg);
    return (temp >> 8) | (temp << 8);
}

/*!
 *   @brief  Reads a signed 16 bit value over I2C/SPI
 */
int16_t BMP280::readS16(uint8_t reg) { return (int16_t)read16(reg); }

int16_t BMP280::readS16_LE(uint8_t reg) {
    return (int16_t)read16_LE(reg);
}

/*!
 *  @brief  Reads a 24 bit value over I2C/SPI
 */
uint32_t BMP280::read24(uint8_t reg) {
    uint8_t buffer[3];
    
    buffer[0] = uint8_t(reg);
    Sensor::write_then_read(_rsid, buffer, 1, buffer, 3, _wire);

    return uint32_t(buffer[0]) << 16 | uint32_t(buffer[1]) << 8 |
           uint32_t(buffer[2]);
}


/*!
 * Reads the temperature from the device.
 * @return The temperature in degress celcius.
 */
float BMP280::readTemperature() {
    int32_t var1, var2;

    int32_t adc_T = read24(BMP280_REGISTER_TEMPDATA);
    adc_T >>= 4;

    var1 = ((((adc_T >> 3) - ((int32_t)_bmp280_calib.dig_T1 << 1))) *
            ((int32_t)_bmp280_calib.dig_T2)) >>
                                             11;

    var2 = (((((adc_T >> 4) - ((int32_t)_bmp280_calib.dig_T1)) *
              ((adc_T >> 4) - ((int32_t)_bmp280_calib.dig_T1))) >> 12) *
            ((int32_t)_bmp280_calib.dig_T3)) >> 14;

    t_fine = var1 + var2;

    float T = (t_fine * 5 + 128) >> 8;
    return T / 100;
}


//todo fix
/*!
 * Reads the barometric pressure from the device.
 * @return Barometric pressure in Pa.
 */
float BMP280::readPressure() {
    int64_t var1, var2, p;

    // Must be done first to get the t_fine variable set up
    readTemperature();


    int32_t adc_P = read24(BMP280_REGISTER_PRESSUREDATA);
    Serial.println(adc_P);
    adc_P >>= 4;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)_bmp280_calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)_bmp280_calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)_bmp280_calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)_bmp280_calib.dig_P3) >> 8) + ((var1 * (int64_t)_bmp280_calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)_bmp280_calib.dig_P1) >> 33;

    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)_bmp280_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)_bmp280_calib.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)_bmp280_calib.dig_P7) << 4);
    return (float)p / 256;
}


/*!
    @brief  calculates the boiling point  of water by a given pressure
    @param pressure pressure in hPa
    @return temperature in °C
*/

float BMP280::waterBoilingPoint(float pressure) {
    // Magnusformular for calculation of the boiling point of water at a given
    // pressure
    return (234.175 * log(pressure / 6.1078)) /
           (17.08085 - log(pressure / 6.1078));
}

void BMP280::setXSettings() {
    setSampling(MODE_NORMAL,
                static_cast<sensor_sampling>(xSettings[2]),
                static_cast<sensor_sampling>(xSettings[2]),
                static_cast<sensor_filter>(xSettings[0]),
                static_cast<standby_duration>(xSettings[1]));
}
