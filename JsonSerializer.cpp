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
    int bufLen =  measureJson(*doc);
    char* buffer = new char[bufLen];

    DynamicJsonDocument *doc = new DynamicJsonDocument(256);
}
