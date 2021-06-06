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
void SensorsIdentifierManager::addSensor(uint8_t address, std::map<uint8_t, Sensor*> * sensors, std::vector<csa::ConflictingAddressStruct*> * conflict) {
    if (numEnumSensorInVectorArray[address].empty()) return;

    if (numEnumSensorInVectorArray[address].size() == 1){
        Serial.print("adding sensor at: ");
        Serial.println(address);
        addSensor(numEnumSensorInVectorArray[address][0], address, sensors, conflict);
    }else {
        Serial.println("failed");
        csa::ConflictingAddressStruct* con;
        con->address = address;
        con->EnumPosOfSensors = numEnumSensorInVectorArray[address];

        for(unsigned int pos: con->EnumPosOfSensors){
            Sensor * s = getSensorPointerForEnumPos(pos, address);
            con->nameOfSensors.emplace_back(s->name());
            delete s;
        }
        conflict->emplace_back(con);
    }


}

void SensorsIdentifierManager::addSensor(unsigned int enumPos, uint8_t address, std::map<uint8_t, Sensor*> * sensors, std::vector<csa::ConflictingAddressStruct*> * conflict){
    //todo delete print
    Serial.print("enumPos:");
    Serial.println(enumPos);
    Serial.flush();
    Sensor* sensor = getSensorPointerForEnumPos(enumPos, address);

    if(sensor != nullptr) {
        sensors->insert(std::pair<uint8_t, Sensor *>(address, sensor));
    } else{
        csa::ConflictingAddressStruct* con;
        con->address = address;
        conflict->emplace_back(con);
    }
}

 Sensor* SensorsIdentifierManager::getSensorPointerForEnumPos(unsigned int enumPos, uint8_t address){
    switch (enumPos){
        case sensorEnum::MPU9250: return (Sensor*)(new class MPU9250(address)); break;
//        case sensorEnum::BMP280: return (sensor*)(new class BMP280); break;

        default: return nullptr; break;
    }
}


void SensorsIdentifierManager::init(){
    char * cArrJson = (char*) spiffs::readFile(SPIFFS, "/SensorAddresses.txt");

    //todo delete debug

    Serial.println("after cArrJson alloc");

    JsonDocument * doc = jp::parseJson(cArrJson);
    Serial.println("after doc creation");
    if(doc != nullptr){
        JsonObjectToArrOfVectors(doc);
        delete doc;
    }

    delete cArrJson;

    Serial.println("end of init");

}

void SensorsIdentifierManager::JsonObjectToArrOfVectors(JsonDocument* doc){
//    JsonObject obj = doc->to<JsonObject>();

    //array 0-127 ie. all i2c addresses the vector contains all sensors that can be on the address
    auto SensorTypeVectors = new std::vector<unsigned int>[128];
    JsonArray arr = doc->as<JsonArray>();

    int i = 0;
    for(JsonArray addressArray : arr){
        for(JsonVariant o : addressArray){
            SensorTypeVectors[i].emplace_back(static_cast<uint8_t>(o.as<unsigned char>()));
        }
        i++;
    }

    //todo delete debug
    for(int i = 0; i < 128; i++){
        Serial.print("[");
        for(unsigned int t : SensorTypeVectors[i]){
            Serial.print(t);
            Serial.print(',');
        }
        Serial.println("]");
    }

    numEnumSensorInVectorArray = SensorTypeVectors;

}
