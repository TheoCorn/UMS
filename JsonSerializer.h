//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_JSONSERIALIZER_H
#define UMDWITHCLASS3_JSONSERIALIZER_H

#include <ArduinoJson.h>
#include "Error.h"


namespace js {

    struct serializeRet{
        char* buff;
        unsigned int bufLen;

        ~serializeRet(){
          delete[] buff;
        }
    };

    js::serializeRet* serializeDoc(JsonDocument * doc);
    js::serializeRet* serializeError(error::Error* error);

}




#endif //UMDWITHCLASS3_JSONSERIALIZER_H
