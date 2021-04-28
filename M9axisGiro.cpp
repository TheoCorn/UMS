
#include "M9axisGiro.hpp"
#include <sstream>

    void M9axisGiro::setUp()  {
      // setting the accelerometer full scale range to +/-8G
      IMU->setAccelRange(MPU9250::ACCEL_RANGE_8G);
      // setting the gyroscope full scale range to +/-500 deg/s
      IMU->setGyroRange(MPU9250::GYRO_RANGE_500DPS);
      // setting DLPF bandwidth to 20 Hz
      IMU->setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_184HZ);
      // setting SRD to 19 for a 50 Hz update rate
      IMU->setSrd(19);
    }

void M9axisGiro::getJson(JsonDocument * ptrDoc, uint8_t uuid)  {
        JsonObject mpuObj = ptrDoc->createNestedObject("MPU9250");
        JsonObject features = mpuObj.createNestedObject("features");

        JsonObject xSettings = mpuObj.createNestedObject("XSettings");

      for (int i = 0; i < 10; i++) {
        features[mpuFeaturesString[i]] = mpuFeaturesBool[i];
      }

    JsonObject accelSettings = xSettings.createNestedObject("accel");
    for (int i = 0; i < 4; i++) {
      accelSettings[accelRangeString[i]] = accelRangeBool[i];
    }

    JsonObject gyroSettings = xSettings.createNestedObject("gyro");
     for (int i = 0; i < 4; i++) {
      gyroSettings[gyroRangeString[i]] = gyroRangeBool[i];
    }    
}

//todo
void M9axisGiro::setJson(JsonVariant * v){

}




void M9axisGiro::readSensor(JsonDocument * ptrDoc)  {
        JsonObject rData = ptrDoc->createNestedObject(name());
        IMU->readSensor();
        for (int i = 0; i < 10; i++) {
          if (mpuFeaturesBool[i]) {
            rData[mpuFeaturesString[i]] = IMU->callReadingFun(IMU->mpuFeaturesFloat[i]);
           }
        }
}

String M9axisGiro::getStringForDisplay(){
  IMU->readSensor();
  String s;
  s = name();
  s += "\t";
  for(int i = 0; i<10; i++){
    float f = IMU->callReadingFun(IMU->mpuFeaturesFloat[i]);
//    ostringstream ss;
//    ss << f;
//    s += ss.str();

      s += (String) f;
  }

  return s;
}

M9axisGiro::M9axisGiro(byte vIndex) : Sensor(){
    address = vIndex;
    IMU = new MPU9250(Wire, address);
}
    
