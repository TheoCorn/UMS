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

void Sensor::JsonSetter(JsonObject& sConf,
                        std::vector<bool>& activeFeaturesVec,
                        std::vector<unsigned int>& xSettings) {

    serializeJson(sConf, Serial);

    JsonArray features = sConf["activeFeatures"];
    JsonArray locXSettings = sConf["XSettings"];
//    JsonArray ISettings = sConf["ISettings"];

    activeFeaturesVec.clear();
    xSettings.clear();
    serializeJson(features, Serial);
    Serial.println(features.size());

    for(JsonVariant v : features){
        activeFeaturesVec.emplace_back(v.as<bool>());
        Serial.println(v.as<bool>());
    }

    for (JsonVariant v : locXSettings) {
        xSettings.emplace_back(v.as<unsigned int>());
    }
}

void Sensor::savedSettingsLoader(const char *filename, std::vector<bool> &activeFeaturesVec,
                                 std::vector<unsigned int> &xSettings) {
    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);

    serializeJson(*doc, Serial);

    if (doc != nullptr) {
        JsonObject obj = doc->to<JsonObject>();
        JsonSetter(obj, activeFeaturesVec, xSettings);
    }else{
        throw std::invalid_argument("invalid file");
    }

    delete[] cArrJson;

}

void Sensor::templatedRead(JsonArray &jra, std::vector<bool> &activeFeaturesVec,
                           uint32_t rsid, Sensor* sensor) {
    char rsidStr[11];
    itoa(rsid, rsidStr, 10);

    JsonObject rData = jra.createNestedObject();
    rData["rsid"] = rsidStr;
    JsonArray values = rData.createNestedArray("values");

    for (int i = 0; i < 10; i++) {
        if (activeFeaturesVec[i]) values.add(sensor->readFeature(i));
    }
}

String Sensor::templatedExtendedString4Display(std::vector<bool>& activeFeaturesVec,
                                               Sensor* sensor,
                                               char const** FeaturesString) {

    Serial.println("tesd");

    for (bool b : activeFeaturesVec ) {
        Serial.print(b);
    }
    Serial.print('\n');

    String s;

    for (int i = 0; i < activeFeaturesVec.size(); i++) {
        float f = sensor->readFeature(i);

        Serial.println(f);

        char cBuffer[64];
        int ret = sprintf(&cBuffer[0], "%e", f);
        s += *FeaturesString[i];

        Serial.println(*FeaturesString[i]);

        s += " ";
        s += activeFeaturesVec[i] ? '1' : '0';
        s += " ";
        s += cBuffer;
        s += "\n";
    }

    Serial.println(s);

    return s;
}
