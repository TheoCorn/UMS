

#ifndef _M9axisGyro_h
#define _M9axisGyro_h

//library for MPU9250 9axis gyro
#include "MPU9250.h"
#include "ArduinoJson.h"
#include "Sensor.hpp"
#include <Wire.h>
#include <SPI.h>
#include <vector>

using namespace std;

class M9axisGiro : public Sensor{
  private:
    MPU9250 *IMU;

    String accelRangeString[4] = { "2g", "4G", "8G", "16G" };
    MPU9250::AccelRange accelRangeEnum[4] = { MPU9250::ACCEL_RANGE_2G, MPU9250::ACCEL_RANGE_4G, MPU9250::ACCEL_RANGE_8G, MPU9250::ACCEL_RANGE_16G };
    bool accelRangeBool[4] = { false, false, true, false };

    String gyroRangeString[4] = { "250DPS", "500DPS", "1000DPS", "2000DPS", };
    MPU9250::GyroRange gyroRangeEnum[4] = { MPU9250::GYRO_RANGE_250DPS, MPU9250::GYRO_RANGE_500DPS, MPU9250::GYRO_RANGE_1000DPS, MPU9250::GYRO_RANGE_2000DPS };   
    bool gyroRangeBool[4] = { false, true, false, false };                                                                  

    String mpuFeaturesString[10] = { "accelerometerX", "accelerometerY", "accelerometerZ", "gyroX", "gyroY", "gyroZ", "magnetometerX", "magnetometerY", "magnetometerZ", "temperature"};
    bool mpuFeaturesBool[10] = { true, true, true, true, true, true, true, true, true, false };

    //available i2c addresses for the sensor
//    std::vector<uint8_t> i2cAddresses{0x68};

    byte address;


   public:

    explicit M9axisGiro(byte vIndex);
    ~M9axisGiro() override{
        delete IMU;
    }
        
    String name() override { return "MPU9250"; }
//    std::vector<uint8_t> c() override{ return i2cAddresses; }
    byte currentAddress() override { return address; }
    
    void getJson(JsonDocument * ptrDoc, uint8_t uuid) override;
    
    void setJson(JsonVariant * v) override;

    void setUp() override;

    void readSensor(JsonDocument * ptrDoc) override;

    String getStringForDisplay() override;
};



#endif
