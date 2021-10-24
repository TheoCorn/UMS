#include "Sensor.hpp"
#include "Wire.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include "Error.h"
#include "spiffs.hpp"



//TwoWire Sensor::wire = Wire;

JsonObject Sensor::createSensorObject(JsonArray &doc) {
    return doc.createNestedObject();
}

JsonArray Sensor::createFeaturesArray(JsonObject &obj) {
    return obj.createNestedArray(JSON_KEYWORD_FEATURES);
}

JsonArray Sensor::createXSettingsArray(JsonObject &obj) {
    return obj.createNestedArray(JSON_KEYWORD_X_SETTINGS);
}

JsonArray Sensor::createISettingsArray(JsonObject &obj) {
    return obj.createNestedArray(JSON_KEYWORD_I_SETTINGS);
}

void Sensor::generateFeatures(JsonObject &sensorObj, std::vector<bool> &activeFeatures) {

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    for(bool element : activeFeatures){
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
    obj[JSON_KEYWORD_RSID] = rsid;
    obj[JSON_KEYWORD_SID] = sid;
}



void Sensor::templatedRead(JsonArray &jra, std::vector<bool> &activeFeaturesVec,
                           uint32_t rsid, Sensor* sensor) {
    char rsidStr[11];
    itoa(rsid, rsidStr, 10);

    JsonObject rData = jra.createNestedObject();
    rData[JSON_KEYWORD_RSID] = rsidStr;
    JsonArray values = rData.createNestedArray(JSON_KEYWORD_VALUES);

    for (int i = 0; i < activeFeaturesVec.size(); i++) {
        if (activeFeaturesVec[i]) values.add(sensor->readFeature(i));
    }
}

String Sensor::templatedExtendedString4Display(std::vector<bool>& activeFeaturesVec,
                                               Sensor* sensor,
                                               char const** FeaturesString) {

    String s;



    for (int i = 0; i < activeFeaturesVec.size(); i++) {
        float f = sensor->readFeature(i);


        char cBuffer[64];
        int ret = sprintf(&cBuffer[0], "%e", f);
        s += FeaturesString[i];


        s += " ";
        s += activeFeaturesVec[i] ? '1' : '0';
        s += " ";
        s += cBuffer;
        s += "\n";
    }

//    Serial.println(s);

    return s;
}

void Sensor::xSettings_JsonSetter(JsonObject &sConf, std::vector<unsigned int> &xSettings) {
    JsonArray locXSettings = sConf[JSON_KEYWORD_X_SETTINGS];
    xSettings.clear();
    for (JsonVariant v : locXSettings) {
        xSettings.emplace_back(v.as<unsigned int>());
    }
}

void Sensor::iSettings_JsonSetter(JsonObject &sConf, std::vector<bool> &iSettings) {
    JsonArray locISettings = sConf[JSON_KEYWORD_I_SETTINGS];
    iSettings.clear();
    for (JsonVariant v : locISettings) {
        iSettings.emplace_back(v.as<bool>());
    }
}

void Sensor::features_JsonSetter(JsonObject &sConf, std::vector<bool> &activeFeaturesVec) {
    JsonArray features = sConf[JSON_KEYWORD_FEATURES];
    Serial.println("features");
    for (auto el : activeFeaturesVec){
        Serial.println(el);
    }
    activeFeaturesVec.clear();
    for(JsonVariant v : features){
        activeFeaturesVec.emplace_back(v.as<bool>());
    }
}

void Sensor::JsonSetter(JsonObject &sConf, std::vector<bool> &activeFeaturesVec) {
    features_JsonSetter(sConf, activeFeaturesVec);

}

void Sensor::JsonSetter(JsonObject &sConf,
                        std::vector<bool>& activeFeaturesVec,
                        std::vector<unsigned int>& xSettings) {


    features_JsonSetter(sConf, activeFeaturesVec);
    xSettings_JsonSetter(sConf, xSettings);

}

void Sensor::JsonSetter(JsonObject &sConf, std::vector<bool> &activeFeaturesVec,
                        std::vector<unsigned int> &xSettings,
                        std::vector<bool> &iSettings) {

    features_JsonSetter(sConf, activeFeaturesVec);
    xSettings_JsonSetter(sConf, xSettings);
    iSettings_JsonSetter(sConf, iSettings);

}

void Sensor::JsonSetter(JsonObject &sConf, std::vector<bool> &activeFeaturesVec, std::vector<bool> &iSettings) {
    features_JsonSetter(sConf, activeFeaturesVec);
    iSettings_JsonSetter(sConf, activeFeaturesVec);
}

void Sensor::savedSettingLoader(const char *filename, std::vector<bool> &activeFeaturesVec) {

    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);


    if (doc != nullptr) {
        JsonObject obj = doc->as<JsonObject>();
        JsonSetter(obj, activeFeaturesVec);
    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

    delete[] cArrJson;
}

void Sensor::savedSettingsLoader(const char *filename, std::vector<bool> &activeFeaturesVec,
                                 std::vector<unsigned int> &xSettings) {

    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);


    if (doc != nullptr) {
        JsonObject obj = doc->as<JsonObject>();
        JsonSetter(obj, activeFeaturesVec, xSettings);
        activeFeaturesVec.shrink_to_fit();
        xSettings.shrink_to_fit();
    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

    delete[] cArrJson;

}

void Sensor::savedSettingsLoader(const char *filename, std::vector<bool> &activeFeaturesVec,
                                 std::vector<bool> &iSettings) {

    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);


    if (doc != nullptr) {
        JsonObject obj = doc->as<JsonObject>();
        JsonSetter(obj, activeFeaturesVec, iSettings);
    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

    delete[] cArrJson;
}

void Sensor::savedSettingsLoader(const char *filename, std::vector<bool> &activeFeaturesVec,
                                 std::vector<unsigned int> &xSettings, std::vector<bool> &iSettings) {

    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);


    if (doc != nullptr) {
        JsonObject obj = doc->as<JsonObject>();
        JsonSetter(obj, activeFeaturesVec, xSettings, iSettings);
    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

    delete[] cArrJson;

}
