#include "Sensor.hpp"
#include "Wire.h"
#include "ArduinoJson.h"


//TwoWire Sensor::wire = Wire;

JsonObject Sensor::createSensorObject(JsonDocument *doc) {
    return doc->createNestedObject("Sensor");
}

JsonObject Sensor::createFeaturesArray(JsonObject &obj) {
    return obj.createNestedArray("Features");
}

JsonObject Sensor::createXSettingsArray(JsonObject &obj) {
    return obj.createNestedArray("XSettings");
}

JsonObject Sensor::createISettingsArray(JsonObject &obj) {
    return obj.createNestedArray("ISettings");
}

void Sensor::generateFeatures(JsonObject &sensorObj, std::vector<bool> &activeFeatures) {

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    for(bool const element : activeFeatures){
        featuresObj.add(element);
    }

}

void Sensor::generateXSettings(JsonObject& sensorObj, std::vector<unsigned int> &xSettings){

    JsonArray xSettingsObj = createXSettingsArray(sensorObj);
    for (auto const &xs : xSettings) {
        xSettingsObj.add(xs);
    }
}


void Sensor::generateISettings(JsonObject& sensorObj, std::vector<bool> &iSettings){

    JsonArray iSettingsObj = createISettingsArray(sensorObj);
    for(bool const is : iSettings){
        iSettingsObj.add(is);
    }
}


void Sensor::generateTemplatedSensorObject(JsonDocument *doc, const uint32_t &rsid, const uint32_t &sid,
                                           const bool isActive) {

    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    active.add(isActive);

}

void Sensor::generateTemplatedSensorObject(JsonDocument *doc, const uint32_t& rsid, const uint32_t& sid,
                                           const unsigned int& xSetting){

    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    active.add(true);

    JsonArray xSettingsObj = createXSettingsArray(sensorObj);
    //todo fix nonsense
    unsigned int b = xSetting;
    xSettingsObj.add(b)

}


void Sensor::generateTemplatedSensorObject(JsonDocument *doc, const uint32_t& rsid, const uint32_t& sid,
                                           std::vector<String> &features, std::vector<bool> &activeFeatures,
                                           std::vector<unsigned int> &xSettings
                                           ) {
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    generateFeatures(sensorObj, activeFeatures);
    generateXSettings(sensorObj, xSettings);

}

void Sensor::generateTemplatedSensorObject(JsonDocument *doc, const uint32_t& rsid, const uint32_t& sid,
                                           std::vector<String> &features, std::vector<bool> &activeFeatures,
                                           std::vector<unsigned int> &xSettings, std::vector<unsigned int> &iSettings
) {
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    generateFeatures(sensorObj, activeFeatures);
    generateXSettings(sensorObj, xSettings);
    generateISettings(sensorObj, iSettings);
}


void Sensor::fillBasicInfo(JsonObject& obj, const uint32_t& rsid, const uint32_t& sid) {
    obj["rsid"] = rsid;
    obj["sid"] = sid;
}

