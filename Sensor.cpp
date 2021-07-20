#include "Sensor.hpp"
#include "Wire.h"
#include "ArduinoJson.h"


//TwoWire Sensor::wire = Wire;

JsonObject Sensor::createSensorObject(JsonArray &doc) {
    return doc.createNestedObject();
}

JsonArray Sensor::createFeaturesArray(JsonObject &obj) {
    return obj.createNestedArray("Features");
}

JsonArray Sensor::createXSettingsArray(JsonObject &obj) {
    return obj.createNestedArray("XSettings");
}

JsonArray Sensor::createISettingsArray(JsonObject &obj) {
    return obj.createNestedArray("ISettings");
}

void Sensor::generateFeatures(JsonObject &sensorObj, std::vector<bool> &activeFeatures) {

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    for(bool element : activeFeatures){
        featuresObj.add(element);
        Serial.println(element);
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


void Sensor::generateTemplatedSensorObject(JsonArray&doc, const uint32_t &rsid, const uint32_t &sid,
                                           const bool& isActive) {

    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    featuresObj.add(isActive);

}

void Sensor::generateTemplatedSensorObject(JsonArray&doc, const uint32_t &rsid, const uint32_t &sid,
                                           const unsigned int &xSetting, const bool &isActive) {

    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    featuresObj.add(isActive);

    JsonArray xSettingsObj = createXSettingsArray(sensorObj);
    //todo fix nonsense
    unsigned int b = xSetting;
    xSettingsObj.add(b);

}


void Sensor::generateTemplatedSensorObject(JsonArray &doc, const uint32_t &rsid, const uint32_t &sid,
                                           std::vector<bool> &activeFeatures, std::vector<unsigned int> &xSettings) {
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    generateFeatures(sensorObj, activeFeatures);
    generateXSettings(sensorObj, xSettings);

}

void Sensor::generateTemplatedSensorObject(JsonArray &doc, const uint32_t &rsid, const uint32_t &sid,
                                           std::vector<bool> &activeFeatures, std::vector<unsigned int> &xSettings, std::vector<bool> &iSettings) {
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
