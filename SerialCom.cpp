//
// Created by theo on 5/9/2021.
//

#include "SerialCom.h"
#include <ArduinoJson.h>
#include "ConflictingSensors.hpp"
#include <string>

#define CONFLICT_SIZE 256


std::string* SerialCom::ConflictsToC_str(std::vector<csa::ConflictingAddressStruct*>* conflicts){
    DynamicJsonDocument doc = DynamicJsonDocument(conflicts->size() * CONFLICT_SIZE);
    JsonArray conflictsArr = doc.createNestedArray("C");

    for(int i = 0; i < conflicts->size(); ++i){
        JsonObject obj =  conflictsArr.createNestedObject();

        obj["a"] = conflicts->at(i)->address;

        JsonArray enumArr = obj.createNestedArray("e");
        JsonArray nameArr = obj.createNestedArray("n");

        for(unsigned int pos = 0; pos < conflicts->EnumPosOfSensors.size(); ++pos){
            enumArr.add(conflicts->at(i)->EnumPosOfSensors.at(pos));
            nameArr.add(conflicts->at(i)->nameOfSensors.at(pos));
        }

    }
    auto output = new std::string;
    serializeJson(doc, *output);

    return output;
}


