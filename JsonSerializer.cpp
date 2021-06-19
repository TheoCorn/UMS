//
// Created by theo on 5/9/2021.
//

#include "JsonSerializer.h"
#include <ArduinoJson.h>
#include "Error.h"

js::serializeRet* js::serializeDoc(JsonDocument* doc){

    int bufLen =  measureJson(*doc);
    char* buffer = new char[bufLen];

    serializeJson(*doc, buffer, bufLen);

    js::serializeRet* sr;
    sr->bufLen = bufLen;
    sr->buff = buffer;

    return sr;
}

js::serializeRet* serializeError(error::Error* error){
    DynamicJsonDocument *doc = new DynamicJsonDocument(256);

    JsonObject errObj = doc->createNestedObject("e");
    errObj["name"] = error->name;
    errObj["message"] = error->message;
    errObj["cad"] = error->createAlertDialog;
    errObj["importance"] = error->importance;

    int bufLen =  measureJson(*doc);
    char* buffer = new char[bufLen];

    serializeJson(*doc, buffer, bufLen);

    js::serializeRet* sr;
    sr->bufLen = bufLen;
    sr->buff = buffer;

    return sr;
}
