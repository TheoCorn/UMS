
#ifndef JsonParserFunctions_h
#define JsonParserFunctions_h

#include <map>
#include <ArduinoJson.h>

#define capacity 8192


namespace jp {

    void parseJson(const char * buffer, void(*mDo)(JsonDocument*, void(JsonPair*)), void(*actualDo)(JsonPair*));

    void cycleThruObj(JsonDocument * doc, void (*actualDo)(JsonPair*));

    void parseJsonWithCycleThru(std::vector<char> * btBuffer, void (*actualDo)(JsonPair*));



}

#endif
