#include "Sensor.hpp"
#include "Wire.h"
#include "ArduinoJson.h"


//TwoWire Sensor::wire = Wire;

JsonObject Sensor::createSensorObject(JsonDocument *doc) {
    return doc->createNestedObject("Sensor");
}

JsonObject Sensor::createFeaturesObject(JsonObject &obj) {
    return obj.createNestedObject("Features");
}

JsonObject Sensor::createXSettingsObject(JsonObject &obj) {
    return obj.createNestedObject("XSettings");
}

JsonObject Sensor::createISettingsObject(JsonObject &obj) {
    return obj.createNestedObject("ISettings");
}

void Sensor::generateFeatures(JsonObject& sensorObj, std::vector<String> &features, std::vector<bool> &activeFeatures){
    JsonObject featuresObj = createFeaturesObject(sensorObj);

    JsonArray featuresArr = sensorObj.createNestedArray("features");
    JsonArray active = sensorObj.createNestedArray("active");

    for (auto const &element : features) {
        featuresArr.add(element);
    }
    for(auto const &element : activeFeatures){
        active.add(element);
    }

}

void Sensor::generateXSettings(JsonObject& sensorObj, std::vector<XSetting> &xSettings){

    JsonObject xSettingsObj = createXSettingsObject(sensorObj);
    for (auto const &xs : xSettings) {
        xSettingBuilder(xs, xSettingsObj);
    }
}


void Sensor::generateISettings(JsonObject& sensorObj, std::vector<ISetting> &iSettings){

    JsonObject iSettingsObj = createISettingsObject(sensorObj);
    for( auto const& is : iSettings){
        iSettingBuilder(is, iSettingsObj);
    }

}


void Sensor::generateTemplatedSensorObject(JsonDocument *doc, const String &name, const uint8_t &uuid) {
    generateTemplatedSensorObject(doc, name, uuid, name);
}

void Sensor::generateTemplatedSensorObject(JsonDocument *doc, const String &name, const uint8_t &uuid,
                                           const String &feature) {
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, name, uuid);

    //Features
    JsonObject featuresObj = createFeaturesObject(sensorObj);
    JsonArray features = featuresObj.createNestedArray("features");
    JsonArray active = featuresObj.createNestedArray("active");

    features.add(feature);
    active.add(true);
}

void Sensor::generateTemplatedSensorObject(JsonDocument *doc, String& name, uint8_t uuid, String feature,
                                   XSetting& xSetting){

    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, name, uuid);

    JsonObject featuresObj = createFeaturesObject(sensorObj);
    JsonArray features = featuresObj.createNestedArray("features");
    JsonArray active = featuresObj.createNestedArray("active");

    features.add(feature);
    active.add(true);

    JsonObject xSettingsObj = createXSettingsObject(sensorObj);
    xSettingBuilder(xSetting, xSettingsObj);

}


void Sensor::generateTemplatedSensorObject(JsonDocument *doc, String &name, uint8_t &uuid,
                                           std::vector<String> &features, std::vector<bool> &activeFeatures,
                                           std::vector<XSetting> &xSettings
                                           ) {
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, name, uuid);

    generateFeatures(sensorObj, features, activeFeatures);
    generateXSettings(sensorObj, xSettings);

}

void Sensor::generateTemplatedSensorObject(JsonDocument *doc, String &name, uint8_t &uuid,
                                           std::vector<String> &features, std::vector<bool> &activeFeatures,
                                           std::vector<XSetting> &xSettings, std::vector<ISetting> &iSettings
) {
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, name, uuid);

    generateFeatures(sensorObj, features, activeFeatures);
    generateXSettings(sensorObj, xSettings);
    generateISettings(sensorObj, iSettings);
}


void Sensor::fillBasicInfo(JsonObject& obj, const String& name, const uint8_t& uuid) {
    obj["name"] = name;
    obj["uuid"] = uuid;
}

void Sensor::xSettingBuilder(const XSetting& xSetting, JsonObject& xSettingsObj){
    JsonObject xsjo = xSettingsObj.createNestedObject("xSetting");
    xsjo["name"] = xSetting.name;
    JsonArray optionsArr = xsjo.createNestedArray("options");

    for( auto const& option : xSetting.options){
        optionsArr.add(option);
    }

    xsjo["active"] = xSetting.active;
}

void Sensor::iSettingBuilder(const ISetting& iSetting, JsonObject& iSettingsObj) {
    JsonObject isjo = xSettingsObj.createNestedObject("iSetting");
    isjo["name"] = iSettingsObj.createNestedObject("iSetting");
    isjo["active"] = iSetting.isActive;
}
