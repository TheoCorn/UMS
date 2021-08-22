//
// Created by theo on 4/24/2021.
//

#include "findSensors.h"
#include <map>
#include <vector>
#include "AddressIdentifier.hpp"
#include <algorithm>
#include "sysInfo.h"



//looks for all i2c devices
void ss::checkI2C(std::vector<csa::ConflictingAddressStruct*> * conflict, std::map<uint32_t , Sensor*> * sensors, SensorsIdentifierManager * sim) {
    //addresses used by previously allocated sensors
    std::vector<uint32_t> usedAddreses;
    //addresses used by previously allocated sensors that are still present
//    std::vector<byte> collUnAddresses;
    bool addreses[128];

    for (bool & address : addreses) {
        address = true;
    }

    //make screen address unavailable
    addreses[sysInfo::screenAddress] = false;

    //make conn devices unavailable
    uint32_t key;
    Sensor* value;
    for(auto& mPair: *sensors){
        std::tie(key, value) = mPair;
        usedAddreses.emplace_back(value->currentAddress());
        addreses[value->currentAddress()] = false;
    }


    for (size_t address = 0; address < 128; address++) {
        Wire.beginTransmission(address);
        uint32_t error = Wire.endTransmission();

        if (error == 0 || error == 4) {
            if (addreses[address]) {
                sim->addSensor(address, sensors, conflict);
            } else if (address != sysInfo::screenAddress) {
//                collUnAddresses.emplace_back(address);
                auto pos = std::find(usedAddreses.begin(), usedAddreses.end(), address);
                if(pos != usedAddreses.end()) usedAddreses.erase(pos);
            }
        }
    }

//    if (usedAddreses.size() != collUnAddresses.size()) {
//        for (int i = 0; i < usedAddreses.size(); i++) {
//            bool exists = false;
//            for (unsigned char collUnAddress : collUnAddresses) {
//                if (usedAddreses[i] == collUnAddress) {
//                    exists = true;
//                    break;
//                }
//            }
//
//            if (!exists) {
//                sensors->erase(sensors->begin() + i);
//            }
//        }
//    }
    for(uint32_t address : usedAddreses){
        auto pos = sensors->find(address);
        sensors->erase(pos);
    }

}
