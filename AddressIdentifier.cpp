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
#include "UnknownSensor.h"
#include "ADS1X15.h"

#define SENSOR_SID_ARRAY_FILE "/SensorAddresses.json"


/**
adds the the correct sensor type to the specified vector

@param address the address of the i2c device
@param vector std::vector<Sensor> pointer with the sensor will be added to
*/
void SensorsIdentifierManager::addSensor(uint32_t address, std::map<uint32_t, Sensor *> *sensors,
                                         std::vector<csa::ConflictingAddressStruct *> *conflicts) {

    if (numEnumSensorInVectorArray[address].size() == 1) {
        addSensor(numEnumSensorInVectorArray[address][0], address, sensors);
    } else if (numEnumSensorInVectorArray[address].empty()){
        sensors->insert(std::pair<uint32_t, Sensor *>(address, ( (Sensor *) new UnknownSensor(address)) ) );
    }
    else {

        for(auto& conf : *conflicts){
            if (conf->address == address) return;
        }

        //todo make more efficient
        auto *con = new csa::ConflictingAddressStruct();
        con->address = address;
        con->EnumPosOfSensors = numEnumSensorInVectorArray[address];

        for (unsigned int pos: con->EnumPosOfSensors) {
            Sensor *s = getSensorPointerForEnumPos(pos, address);
            con->nameOfSensors.emplace_back(s->name());
            delete s;
        }
        conflicts->emplace_back(con);
    }


}

void SensorsIdentifierManager::addSensor(unsigned int enumPos, uint32_t address, std::map<uint32_t, Sensor *> *sensors) {

    Sensor *sensor = getSensorPointerForEnumPos(enumPos, address);
    sensors->insert(std::pair<uint32_t, Sensor *>(address, sensor));
}

Sensor *SensorsIdentifierManager::getSensorPointerForEnumPos(unsigned int enumPos, uint32_t address) {
    switch (enumPos) { // NOLINT(hicpp-multiway-paths-covered)
        case sensorEnum::MPU9250: return dynamic_cast<Sensor*>(new class MPU9250(address)); break;
        case sensorEnum::ADS1015: return dynamic_cast<Sensor*>(new class ADS1015(address)); break;
        case sensorEnum::ADS1115: return dynamic_cast<Sensor*>(new class ADS1115(address)); break;
//        case sensorEnum::BMP280: return (sensor*)(new class BMP280); break;

        default: return dynamic_cast<Sensor*>(new class UnknownSensor(address)); break;
    }
}


void SensorsIdentifierManager::init() {
    char *cArrJson = (char *) spiffs::readFile(SPIFFS, SENSOR_SID_ARRAY_FILE);

    JsonDocument *doc = jp::parseJson(cArrJson);
    if (doc != nullptr) {
        //doc is deleted by JsonObjectToArrOfVectors function
        JsonObjectToArrOfVectors(doc);
    }

    delete cArrJson;
}

void SensorsIdentifierManager::JsonObjectToArrOfVectors(JsonDocument *doc) {
//    JsonObject obj = doc->to<JsonObject>();

    //array 0-127 ie. all i2c addresses the vector contains all sensors that can be on the address
    numEnumSensorInVectorArray = new std::vector<unsigned int>[128];
    JsonArray arr = doc->as<JsonArray>();

    int i = 0;
    for (JsonArray addressArray : arr) {
        for (JsonVariant o : addressArray) {
            numEnumSensorInVectorArray[i].emplace_back(o.as<unsigned int>());
        }
        i++;
    }

}
