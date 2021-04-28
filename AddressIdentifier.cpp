//
// Created by theo on 4/23/2021.
//

#include "AddressIdentifier.hpp"
#include "ConflictingSensors.hpp"
#include "JsonParserFunctions.hpp"
#include "spiffs.hpp"
#include "sensorEnum.h"
#include "M9axisGiro.hpp"



/*
adds the the correct sensor type to the specified vector

@param address the address of the i2c device
@param vector std::vector<Sensor> pointer with the sensor will be added to
*/
void SensorsIdentifierManager::addSensor(byte address, std::map<uint8_t, Sensor*> * sensors, std::vector<csa::ConflictingAddressStruct> * conflict) {

    if (numEnumSensorInVectorArray[address].size() == 1){
        addSensor(numEnumSensorInVectorArray[address][0], address, sensors);
    }else{

    }


}

void SensorsIdentifierManager::addSensor(unsigned int enumPos, byte address, std::map<uint8_t, Sensor*> * sensors){
    Sensor * sensor;
    switch (enumPos){
        case sensorEnum::MPU9250: sensor = new M9axisGiro(address);
    }
}


void SensorsIdentifierManager::init(){
    char * cArrJson = spiffs::readFile(SPIFFS, "/SensorAddresses.txt");
    jp::parseJson(cArrJson, &JsonObjectToArrOfVectors);
}

void SensorsIdentifierManager::JsonObjectToArrOfVectors(JsonObject* obj, void (*actaulDo)(JsonPair*)){
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
