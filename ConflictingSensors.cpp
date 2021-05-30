//
// Created by theo on 5/29/2021.
//

#include <ArduinoJson.h>
#include "ConflictingSensors.hpp"

#define CONFLICT_SIZE 256

JsonDocument* csa::conflictsToString(std::vector<csa::ConflictingAddressStruct*>* conflicts){
    auto doc = new DynamicJsonDocument(conflicts->size() * CONFLICT_SIZE);
    JsonArray conflictsArr = doc->createNestedArray("C");

    for(int i = 0; i < conflicts->size(); ++i){
        JsonObject obj =  conflictsArr.createNestedObject();

        obj["a"] = conflicts->at(i)->address;

        JsonArray enumArr = obj.createNestedArray("e");
        JsonArray nameArr = obj.createNestedArray("n");

        for(unsigned int pos = 0; pos < conflicts->at(i)->EnumPosOfSensors.size(); ++pos){
            enumArr.add(conflicts->at(i)->EnumPosOfSensors.at(pos));
            nameArr.add(conflicts->at(i)->nameOfSensors.at(pos));
        }

    }

    return doc;

}
