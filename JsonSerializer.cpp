//
// Created by theo on 5/9/2021.
//

#include "JsonSerializer.h"
#include <ArduinoJson.h>

js::serializeRet js::serializeDoc(JsonDocument * doc){

    int bufLen =  measureJson(*doc);
    char *buffer = new char[bufLen];

    serializeJson(*doc, buffer, bufLen);

    js::serializeRet * sr = new js::serializeRet;
    sr->bufLen = bufLen;
    sr->buff = buffer;

    return sr;
}
