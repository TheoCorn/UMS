//
// Created by theo on 5/29/2021.
//

#include <ArduinoJson.h>
#include "ConflictingSensors.hpp"

#define CONFLICT_SIZE 256

JsonDocument* csa::conflictsToJsonDoc(std::vector<csa::ConflictingAddressStruct*>* conflicts){
    auto doc = new DynamicJsonDocument(conflicts->size() * CONFLICT_SIZE);
    JsonArray conflictsArr = doc->createNestedArray(C_JSON_KEYWORD_CONFLICTS_ARRAY);

    for(auto & conflict : *conflicts){
        JsonObject obj =  conflictsArr.createNestedObject();

        obj[JSON_KEYWORD_RSID] = conflict->rsid;

        JsonArray sidsArray = obj.createNestedArray(C_JSON_KEYWORD_SIDS);
//        JsonArray nameArr = obj.createNestedArray();

//        for(unsigned int pos = 0; pos < conflicts->at(i)->sids.size(); ++pos){
//            enumArr.add(conflicts->at(i)->sids.at(pos));
//        }

        for(auto m_sid : conflict->sids){
            sidsArray.add(m_sid);
        }

    }

    return doc;

}
