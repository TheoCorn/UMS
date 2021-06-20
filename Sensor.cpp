#include "Sensor.h"
#include "Wire.h"
#include "ArduinoJson.h"


//TwoWire Sensor::wire = Wire;

JsonObject Sensor::createSensorObject(JsonDocument* doc) {
    return doc->createNestedObject("Sensor");
}