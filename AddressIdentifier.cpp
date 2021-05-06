//
// Created by theo on 4/23/2021.
//

#include "AddressIdentifier.hpp"
#include "ConflictingSensors.hpp"
#include "JsonParserFunctions.hpp"
#include "spiffs.hpp"
#include "sensorEnum.h"
#include "MPU9250.hpp"
#include <ArduinoJson.h>



/*
adds the the correct sensor type to the specified vector

@param address the address of the i2c device
@param vector std::vector<Sensor> pointer with the sensor will be added to
*/
void SensorsIdentifierManager::addSensor(uint8_t address, std::map<uint8_t, Sensor*> * sensors, std::vector<csa::ConflictingAddressStruct> * conflict) {

    if (numEnumSensorInVectorArray[address].size() == 1){
        addSensor(numEnumSensorInVectorArray[address][0], address, sensors);
    }else{
        csa::ConflictingAddressStruct con;
        con.address = address;
        con.EnumPosOfSensors = numEnumSensorInVectorArray[address];

        for(unsigned int pos: con.EnumPosOfSensors){
            Sensor * s = getSensorPointerForEnumPos(pos, address);
            con.nameOfSensors.emplace_back(s->name());
            delete s;
        }
        conflict->emplace_back(con);
    }


}

void SensorsIdentifierManager::addSensor(unsigned int enumPos, uint8_t address, std::map<uint8_t, Sensor*> * sensors){

    sensors->insert(std::pair<uint8_t, Sensor*>(address, getSensorPointerForEnumPos(enumPos, address)));
}

 Sensor* SensorsIdentifierManager::getSensorPointerForEnumPos(unsigned int enumPos, uint8_t address){
    switch (enumPos){
        case sensorEnum::MPU9250: return (Sensor*)(new class MPU9250(address)); break;
    }
}


void SensorsIdentifierManager::init(){
    char * cArrJson = (char*) spiffs::readFile(SPIFFS, "/SensorAddresses.txt");
    jp::parseJson(cArrJson, &JsonObjectToArrOfVectors, nullptr);
}

void SensorsIdentifierManager::JsonObjectToArrOfVectors(JsonObject* obj, void (*actualDo)(JsonPair*)){
    //array 0-127 ie. all i2c addresses the vector contains all sensors that can be on the address
    std::vector<unsigned int> *SensorTypeVectors = new std::vector<unsigned int>[128];
    JsonArray arr = obj->as<JsonArray>();

    int i = 0;
    for(JsonArray addressArray : arr){
        for(JsonVariant o : addressArray){
            SensorTypeVectors[i].emplace_back(o.as<unsigned int>());
        }
        i++;
    }

    numEnumSensorInVectorArray = SensorTypeVectors;

}
