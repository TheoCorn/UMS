//
// Created by theo on 4/24/2021.
//

#include <vector>
#include "JsonParserFunctions.hpp"


void jp::cycleThruObj (JsonDocument * doc, std::function<void(JsonPair*)>& actualDo) {
    JsonObject obj = doc->as<JsonObject>();
    for (JsonPair p : obj) {
        actualDo(&p);
    }
}

/**
   deserializes json and starts the process of unpacking the Json
   earpieces '\0' to the end of the vector

   @param btBuffer * std::vector<char> pointer to buffer of incoming Json
   @param do function pointer to the function with will do stuff with the elements

   @exception DeserializationError
*/
void jp::parseJsonWithCycleThru (std::vector<char> * btBuffer, std::function<void(JsonPair*)>& actualDo) {

    btBuffer->emplace_back('\0');
    const char * toPass = btBuffer->data();
    std::function<void(JsonDocument*, std::function<void(JsonPair*)>&)> cycleThru = jp::cycleThruObj;
    parseJson(toPass, cycleThru, actualDo);

}
/**
 * deserializes the Json call the function cycleFun points to that takes as input the actualDo function Pointer
 * if the second function pointer is not needed pass nullptr
 *
 * @param buffer the serialised Json
 * @param void(*cycleFun)(JsonDocument*, void(JsonPair*))
 * @param void(*actualDo)(JsonPair*)
 */
void jp::parseJson(
        const char * buffer,
        std::function<void(JsonDocument*, std::function<void(JsonPair*)>&)>& cycleFun,
        std::function<void(JsonPair*)>& actualDo){

    JsonDocument *doc = jp::parseJson(buffer);
    cycleFun(doc, actualDo);
//    delete doc;
}

/**
 * parses json and return JsonDocument* the user of the function must manage the lifecycle of the JsonDocument*
 *
 * @param const char* buffer the Serialized Json
 * @return JsonDocument* can be null pointer if the the deserialization failed
 */
JsonDocument * jp::parseJson(const char * buffer){
    DynamicJsonDocument *doc = new DynamicJsonDocument(DEFAULT_JDOC_CAPACITY);
    DeserializationError err = deserializeJson(*doc, buffer);

    if (err != DeserializationError::Ok) {
        //todo implement error message
        return nullptr;
    }

    doc->shrinkToFit();

    return doc;

}
