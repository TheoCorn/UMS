//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_JSONSERIALIZER_H
#define UMDWITHCLASS3_JSONSERIALIZER_H

#include <ArduinoJson.h>
#include "Error.h"
#include "json_key_words.h"


namespace js {

    struct serializeRet{
        const char* buff;
        unsigned int bufLen;

        serializeRet(const char* buff, unsigned int bufLen) : buff(buff), bufLen(bufLen) {}

        ~serializeRet(){
          delete[] buff;
        }
    };

    js::serializeRet* serializeDoc(JsonDocument * doc);
    js::serializeRet* serializeError(error::Error* error);
    //js::serializeRet* serializeObj(JsonObject& obj);


}




#endif //UMDWITHCLASS3_JSONSERIALIZER_H
