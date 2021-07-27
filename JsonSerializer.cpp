//
// Created by theo on 5/9/2021.
//

#include "JsonSerializer.h"
#include <ArduinoJson.h>
#include "Error.h"

js::serializeRet* js::serializeDoc(JsonDocument* doc){
    Serial.println("serializeDoc");

    unsigned int bufLen =  measureJson(*doc) + 1;
    Serial.println(bufLen);
    char* buffer = new char[bufLen];

    Serial.println("serializeDoc");

    serializeJson(*doc, buffer, bufLen);

    Serial.println("serializeDoc");

    auto* sr = new js::serializeRet();
    sr->bufLen = bufLen;
    sr->buff = buffer;

    Serial.write(sr->buff, sr->bufLen);
    Serial.println();

    return sr;
}

js::serializeRet* js::serializeError(error::Error* error){
    DynamicJsonDocument *doc = new DynamicJsonDocument(256);

    JsonObject errObj = doc->createNestedObject("error");
    errObj["name"] = error->name;
    errObj["message"] = error->message;
    errObj["ui"] = error->appearance;
    errObj["importance"] = error->importance;
    errObj["backgroundAction"] = error->backgroundAction;


    int bufLen =  measureJson(*doc);
    char* buffer = new char[bufLen];

    serializeJson(*doc, buffer, bufLen);

    js::serializeRet* sr;
    sr->bufLen = bufLen;
    sr->buff = buffer;

    return sr;
}
