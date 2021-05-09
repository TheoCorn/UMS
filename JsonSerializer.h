//
// Created by theo on 5/9/2021.
//

#ifndef UMDWITHCLASS3_JSONSERIALIZER_H
#define UMDWITHCLASS3_JSONSERIALIZER_H


namespace js{

    void serializeDoc(JsonDocument * doc);

    struct serializeRet{
        char* buff;
        int bufLen;
    };
}


#endif //UMDWITHCLASS3_JSONSERIALIZER_H
