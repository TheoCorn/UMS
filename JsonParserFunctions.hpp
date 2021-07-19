
#ifndef JsonParserFunctions_h
#define JsonParserFunctions_h

#include <map>
#include <ArduinoJson.h>
#include <functional>

#define DEFAULT_JDOC_CAPACITY 8192


namespace jp {

    void parseJson(const char * buffer, std::function<void(JsonDocument*, std::function<JsonPair*>)>& mDo, std::function<void(JsonPair*)>& actualDo);

    void cycleThruObj(JsonDocument * doc, std::function<void(JsonPair*)> actualDo);

    void parseJsonWithCycleThru(std::vector<char> * btBuffer, std::function<void(JsonPair*)>& actualDo);

    JsonDocument * parseJson(const char * buffer);

}

#endif
