//
// Created by theo on 4/24/2021.
//

#include "JsonParserFunctions.hpp"

using namespace jp;

void cycleThruObj (JsonDocument * doc, void (*actualDo)(JsonPair*)) {

    JsonObject obj = doc->to<JsonObject>();
    for (JsonPair p : obj) {
        actualDo(&p);
    }
}

using namespace jp;
/*
   deserializes json and starts the process of unpacking the Json
   earpieces '\0' to the end of the vector

   @param btBuffer * std::vector<char> pointer to buffer of incoming Json
   @param do function pointer to the function with will do stuff with the elements

   @exception DeserializationError
*/
void parseJsonWithCycleThru (std::vector<char> * btBuffer, void (*actualDo)(JsonPair*)) {
//    char data [btBuffer->size()];
//    for (int i = 0; i < btBuffer->size(); i++) {
//        data[i] = btBuffer->at(i);
//    }

    btBuffer->emplace_back('\0');
    char * toPass = &(btBuffer->front());
    parseJson(toPass, &cycleThruObj, actualDo);

}
using namespace jp;

void parseJson(const char * buffer, void(*mDo)(JsonDocument*, void(JsonPair*)), void(*actualDo)(JsonPair*)){
    DynamicJsonDocument doc(capacity);
    DeserializationError err = deserializeJson(doc, buffer);

    if (err != DeserializationError::Ok) {
        log_e("error in deserialization: " + err.c_str());
        return;
    }



    mDo(&doc, actualDo);
}
