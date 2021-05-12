//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_JSONSERIALIZER_H
#define UMDWITHCLASS3_JSONSERIALIZER_H

#include <ArduinoJson.h>


namespace js{

    struct serializeRet{
        char* buff;
        int bufLen;
    };

    js::serializeRet* serializeDoc(JsonDocument * doc);


}


#endif //UMDWITHCLASS3_JSONSERIALIZER_H
