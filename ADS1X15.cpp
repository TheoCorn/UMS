//
// Created by theo on 9/27/2021.
//

#include "ADS1X15.h"

ADS1015::ADS1015(uint32_t address) {
    m_bitShift = 4;
    m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
    m_dataRate = RATE_ADS1015_1600SPS;
    begin(address, &Wire);

}


ADS1115::ADS1115(uint32_t address) {
    m_bitShift = 0;
    m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
    m_dataRate = RATE_ADS1115_128SPS;
    begin(address, &Wire);
}

/**************************************************************************/
/*!
    @brief  Sets up the HW (reads coefficients values, etc.)

    @param i2c_addr I2C address of device
    @param wire I2C bus

    @return true if successful, otherwise false
*/
/**************************************************************************/
bool ADS1X15::begin(uint8_t i2c_addr, TwoWire *wire) {
    m_i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);
    return m_i2c_dev->begin();
}

/**************************************************************************/
/*!
    @brief  Sets the gain and input voltage range

    @param gain gain setting to use
*/
/**************************************************************************/
void ADS1X15::setGain(adsGain_t gain) { m_gain = gain; }


/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel

    @param channel ADC channel to read

    @return the ADC reading
*/
/**************************************************************************/
int16_t ADS1X15::readADC_SingleEnded(uint8_t channel) {
    if (channel > 3) {
        return 0;
    }

    // Start with default values
    uint16_t config =
            ADS1X15_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
            ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
            ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
            ADS1X15_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= m_gain;

    // Set data rate
    config |= m_dataRate;

    // Set single-ended input channel
    switch (channel) {
        case (0):
            config |= ADS1X15_REG_CONFIG_MUX_SINGLE_0;
            break;
        case (1):
            config |= ADS1X15_REG_CONFIG_MUX_SINGLE_1;
            break;
        case (2):
            config |= ADS1X15_REG_CONFIG_MUX_SINGLE_2;
            break;
        case (3):
            config |= ADS1X15_REG_CONFIG_MUX_SINGLE_3;
            break;
    }

    // Set 'start single-conversion' bit
    config |= ADS1X15_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    writeRegister(ADS1X15_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    while (!conversionComplete())
        ;

    // Read the conversion results
    return getLastConversionResults();
}

/**************************************************************************/
/*!
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN0) and N (AIN1) input.  Generates
            a signed value since the difference can be either
            positive or negative.

    @return the ADC reading
*/
/**************************************************************************/
int16_t ADS1X15::readADC_Differential_0_1() {
    // Start with default values
    uint16_t config =
            ADS1X15_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
            ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
            ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
            ADS1X15_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= m_gain;

    // Set data rate
    config |= m_dataRate;

    // Set channels
    config |= ADS1X15_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N

    // Set 'start single-conversion' bit
    config |= ADS1X15_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    writeRegister(ADS1X15_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    while (!conversionComplete())
        ;

    // Read the conversion results
    return getLastConversionResults();
}

/**************************************************************************/
/*!
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN2) and N (AIN3) input.  Generates
            a signed value since the difference can be either
            positive or negative.

    @return the ADC reading
*/
/**************************************************************************/
int16_t ADS1X15::readADC_Differential_2_3() {
    // Start with default values
    uint16_t config =
            ADS1X15_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
            ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
            ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
            ADS1X15_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= m_gain;

    // Set data rate
    config |= m_dataRate;

    // Set channels
    config |= ADS1X15_REG_CONFIG_MUX_DIFF_2_3; // AIN2 = P, AIN3 = N

    // Set 'start single-conversion' bit
    config |= ADS1X15_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    writeRegister(ADS1X15_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    while (!conversionComplete())
        ;

    // Read the conversion results
    return getLastConversionResults();
}

/**************************************************************************/
/*!
    @brief  Sets up the comparator to operate in basic mode, causing the
            ALERT/RDY pin to assert (go from high to low) when the ADC
            value exceeds the specified threshold.

            This will also set the ADC in continuous conversion mode.

    @param channel ADC channel to use
    @param threshold comparator threshold
*/
/**************************************************************************/
void ADS1X15::startComparator_SingleEnded(uint8_t channel,
                                                   int16_t threshold) {
    // Start with default values
    uint16_t config =
            ADS1X15_REG_CONFIG_CQUE_1CONV |   // Comparator enabled and asserts on 1
            // match
            ADS1X15_REG_CONFIG_CLAT_LATCH |   // Latching mode
            ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
            ADS1X15_REG_CONFIG_MODE_CONTIN |  // Continuous conversion mode
            ADS1X15_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

    // Set PGA/voltage range
    config |= m_gain;

    // Set data rate
    config |= m_dataRate;

    // Set single-ended input channel
    switch (channel) {
        case (0):
            config |= ADS1X15_REG_CONFIG_MUX_SINGLE_0;
            break;
        case (1):
            config |= ADS1X15_REG_CONFIG_MUX_SINGLE_1;
            break;
        case (2):
            config |= ADS1X15_REG_CONFIG_MUX_SINGLE_2;
            break;
        case (3):
            config |= ADS1X15_REG_CONFIG_MUX_SINGLE_3;
            break;
    }

    // Set the high threshold register
    // Shift 12-bit results left 4 bits for the ADS1015
    writeRegister(ADS1X15_REG_POINTER_HITHRESH, threshold << m_bitShift);

    // Write config register to the ADC
    writeRegister(ADS1X15_REG_POINTER_CONFIG, config);
}

/**************************************************************************/
/*!
    @brief  In order to clear the comparator, we need to read the
            conversion results.  This function reads the last conversion
            results without changing the config value.

    @return the last ADC reading
*/
/**************************************************************************/
int16_t ADS1X15::getLastConversionResults() {
    // Read the conversion results
    uint16_t res = readRegister(ADS1X15_REG_POINTER_CONVERT) >> m_bitShift;
    if (m_bitShift == 0) {
        return (int16_t)res;
    } else {
        // Shift 12-bit results right 4 bits for the ADS1015,
        // making sure we keep the sign bit intact
        if (res > 0x07FF) {
            // negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
        return (int16_t)res;
    }
}

/**************************************************************************/
/*!
    @brief  Returns true if conversion is complete, false otherwise.

    @param counts the ADC reading in raw counts

    @return the ADC reading in volts
*/
/**************************************************************************/
float ADS1X15::computeVolts(int16_t counts) {
    // see data sheet Table 3
    float fsRange;
    switch (m_gain) {
        case GAIN_TWOTHIRDS:
            fsRange = 6.144f;
            break;
        case GAIN_ONE:
            fsRange = 4.096f;
            break;
        case GAIN_TWO:
            fsRange = 2.048f;
            break;
        case GAIN_FOUR:
            fsRange = 1.024f;
            break;
        case GAIN_EIGHT:
            fsRange = 0.512f;
            break;
        case GAIN_SIXTEEN:
            fsRange = 0.256f;
            break;
        default:
            fsRange = 0.0f;
    }
    return counts * (fsRange / (32768 >> m_bitShift));
}

/**************************************************************************/
/*!
    @brief  Returns true if conversion is complete, false otherwise.
*/
/**************************************************************************/
bool ADS1X15::conversionComplete() {
    return (readRegister(ADS1X15_REG_POINTER_CONFIG) & 0x8000) != 0;
}

/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register

    @param reg register address to write to
    @param value value to write to register
*/
/**************************************************************************/
void ADS1X15::writeRegister(uint8_t reg, uint16_t value) {
    buffer[0] = reg;
    buffer[1] = value >> 8;
    buffer[2] = value & 0xFF;
    m_i2c_dev->write(buffer, 3);
}

/**************************************************************************/
/*!
    @brief  Read 16-bits from the specified destination register

    @param reg register address to read from

    @return 16 bit register value read
*/
/**************************************************************************/
uint16_t ADS1X15::readRegister(uint8_t reg) {
    buffer[0] = reg;
    m_i2c_dev->write(buffer, 1);
    m_i2c_dev->read(buffer, 2);
    return ((buffer[0] << 8) | buffer[1]);
}

void ADS1X15::getJson(JsonArray &jArr, uint32_t rsid, uint32_t sid) {
    Sensor::generateTemplatedSensorObject(jArr, rsid, sid, activeFeaturesVec, xSettings);
}

void ADS1X15::setJson(JsonObject &sConf){
    Sensor::JsonSetter(sConf, activeFeaturesVec, xSettings);
}

void ADS1X15::setUp(const char *filename) {
    Sensor::savedSettingsLoader(filename, activeFeaturesVec, xSettings);
}

void ADS1X15::readSensor(JsonArray &jra, uint32_t rsid, Sensor* sensor) {
    Sensor::templatedRead(jra, activeFeaturesVec, rsid, sensor);
}


String ADS1X15::getExtendedStringForDisplay(Sensor *sensor) {
    return Sensor::templatedExtendedString4Display(activeFeaturesVec, sensor, (const char**) adsFeaturesString);

}

void ADS1X15::m_saveConfig(const char* filepath) {
    Sensor::settingsSaver(filepath, activeFeaturesVec, xSettings);
}

float ADS1X15::readFeature(size_t index) {
    switch (index) {
        case 0:
        case 1:
            return computeVolts(readADC_SingleEnded(index));
            break;
        case 2:
            return computeVolts(readADC_Differential_2_3());
            break;
        default: throw std::invalid_argument(ERROR_MSG__INVALID_ARGUMENT);
    }
}


void ADS1X15::setGain(){
    // gain
    switch (xSettings[0]){
        case 0: setGain(GAIN_TWOTHIRDS); break;
        case 1: setGain(GAIN_ONE); break;
        case 2: setGain(GAIN_TWO); break;
        case 3: setGain(GAIN_FOUR); break;
        case 4: setGain(GAIN_EIGHT); break;
        case 5: setGain(GAIN_SIXTEEN); break;
    }

}


void ADS1015::setConfig() {
    ADS1X15::setGain();
// data rate
    switch (xSettings[1]) {
        case 0:
            m_dataRate = RATE_ADS1015_128SPS;
            break;
        case 1:
            m_dataRate = RATE_ADS1015_250SPS;
            break;
        case 2:
            m_dataRate = RATE_ADS1015_490SPS;
            break;
        case 3:
            m_dataRate = RATE_ADS1015_920SPS;
            break;
        case 4:
            m_dataRate = RATE_ADS1015_1600SPS;
            break;
        case 5:
            m_dataRate = RATE_ADS1015_2400SPS;
            break;
        case 6:
            m_dataRate = RATE_ADS1015_3300SPS;
            break;
    }
}

void ADS1115::setConfig() {
    ADS1X15::setGain();
    // data rate
    switch (xSettings[1]) {
        case 0:
            m_dataRate = RATE_ADS1115_8SPS;
            break;
        case 1:
            m_dataRate = RATE_ADS1115_16SPS;
            break;
        case 2:
            m_dataRate = RATE_ADS1115_32SPS;
            break;
        case 3:
            m_dataRate = RATE_ADS1115_64SPS;
            break;
        case 4:
            m_dataRate = RATE_ADS1115_128SPS;
            break;
        case 5:
            m_dataRate = RATE_ADS1115_250SPS;
            break;
        case 6:
            m_dataRate = RATE_ADS1115_475SPS;
            break;
        case 7:
            m_dataRate = RATE_ADS1115_860SPS;
            break;
    }
}