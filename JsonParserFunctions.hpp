
#ifndef JsonParserFunctions_h
#define JsonParserFunctions_h

#include <map>
#include <ArduinoJson.h>

#define capacity 8192


namespace jp {

    void parseJson(const char * buffer, void(*mDo)(JsonObject*, void(JsonPair*)), void(*actualDo)(JsonPair*));

    void cycleThruObj(JsonObject * obj, void (*actaulDo)(JsonPair*));

    void parseJsonWithCycleThru(std::vector<char> * btBuffer, void (*actaulDo)(JsonPair*));



}

#endif
