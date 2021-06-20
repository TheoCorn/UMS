#include "Sensor.h"
#include "Wire.h"
#include "ArduinoJson.h"


//TwoWire Sensor::wire = Wire;

JsonObject Sensor::createSensorObject(JsonDocument* doc) {
    return doc->createNestedObject("Sensor");
}

JsonObject Sensor::createFeaturesObject(JsonObject& obj){
    return obj.createNestedObject("Features");
}

JsonObject Sensor::createXSettingsObject(JsonObject& obj){
    return obj.createNestedObject("XSettings");
}

JsonObject Sensor::createISettingsObject(JsonObject& obj){
    return obj.createNestedObject("ISettings");
}

void Sensor::generateTemplatedSensorObject(JsonDocument *doc, String& name, uint8_t& uuid){

    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, name, uuid);

    //Features
    JsonObject featuresObj = createFeaturesObject(sensorObj);
    JsonArray features = featuresObj.createNestedArray("features");
    JsonArray active = featuresObj.createNestedArray("active");
    features.add(name);
    active.add(true);

}

void Sensor::generateTemplatedSensorObject(JsonDocument *doc, String name, uint8_t uuid, String feature){
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, name, uuid);


}

void Sensor::fillBasicInfo(JsonObject& obj, String& name, uint8_t& uuid){
    obj["name"] = name;
    obj["uuid"] = uuid;
}