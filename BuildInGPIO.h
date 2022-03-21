

//#ifndef UMDWITHCLASS3_BUILDINGPIO_H
//#define UMDWITHCLASS3_BUILDINGPIO_H
//
//#include "Sensor.hpp"
//#include "sensorEnum.h"
//#include "gpioNames.h"
//
//class BuildInGPIO : public Sensor {
//    uint32_t _rsid;
//    std::vector<bool> activeFeaturesVec;
//
//    static constexpr const char* features_strings[2] = {"5vS", "3vS" };
//    static constexpr const char* jsonFilePath = "/sensorData/8.json";
//
//
//public:
//
//    BuildInGPIO(uint32_t rsid) : _rsid(rsid) { }
//
//    ~BuildInGPIO() override = default;
//
//    String name() override { return "ADC"; };
//
//    uint32_t sid() override { return sensorEnum::BUILDIN_GPIO; };
//
//    void setUp() override;
//
//    void readSensor(JsonArray &jra) override;
//
//    void getJson(JsonArray &jArr) override;
//
//    void setJson(JsonObject &sConf) override;
//
//    uint32_t rsid() override { return _rsid; };
//
//    String getStringForDisplay() override;
//
//    String getExtendedStringForDisplay() override;
//
//    void saveConfig() override;
//
//    float readFeature(size_t index) override;
//};
//
//
//#endif //UMDWITHCLASS3_BUILDINGPIO_H
