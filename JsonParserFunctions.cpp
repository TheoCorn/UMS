//
// Created by theo on 4/24/2021.
//

#include <vector>
#include "JsonParserFunctions.hpp"

using namespace jp;

void jp::cycleThruObj (JsonDocument * doc, void (*actualDo)(JsonPair*)) {

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
void jp::parseJsonWithCycleThru (std::vector<char> * btBuffer, void (*actualDo)(JsonPair*)) {

    btBuffer->emplace_back('\0');
    char * toPass = &(btBuffer->front());
    parseJson(toPass, &jp::cycleThruObj, actualDo);

}
using namespace jp;
/*
 * deserializes the Json call the function mDo points to that takes as input the actualDo function Pointer
 * if the second function pointer is not needed pass nullptr
 *
 * @param buffer the serialised Json
 * @param void(*mDo)(JsonDocument*, void(JsonPair*))
 * @param void(*actualDo)(JsonPair*)
 */
void jp::parseJson(const char * buffer, void(*mDo)(JsonDocument*, void(JsonPair*)), void(*actualDo)(JsonPair*)){
    JsonDocument *doc = jp::parseJson(buffer);
    mDo(doc, actualDo);
    delete doc;
}

using namespace jp;
/*
 * parses json and return JsonDocument* the user of the function must manage the lifecycle of the JsonDocument*
 *
 * @param const char* buffer the Serialized Json
 * @return JsonDocument* can be null pointer if the the deserialization failed
 */
JsonDocument * jp::parseJson(const char * buffer){
    DynamicJsonDocument *doc = new DynamicJsonDocument(capacity);
    DeserializationError err = deserializeJson(*doc, buffer);

    if (err != DeserializationError::Ok) {
//        log_e("error in deserialization: " + err.c_str());
        return nullptr;
    }

    return doc;

}
