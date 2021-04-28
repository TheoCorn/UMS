//
// Created by theo on 4/24/2021.
//

#include "AllAboutSensors/findSensors.h"
#include <map>
#include <vector>


//looks for all i2c devices
void checkI2C(std::vector<csa::ConflictingAddressStruct> * conflict) {

    //addresses used by previusly alocated sensors
    std::vector<byte> usedAddreses;
    //addresses used by previusly alocated sensors that are still pressent
    std::vector<byte> collUnAddresses;
    bool addreses[128];

    for (int i = 0; i < 127; i++) {
        addreses[i] = true;
    }

    //make screen address unavailable
    addreses[SCREEN_I2C_ADDRESS] = false;

    //make conn devices unavailable
    uint8_t key;
    uint8_t i;
    Sensor value;
    for(auto& mPair: sensors){
        std::tie(key, value) = mPair
        usedAddreses.emplace_back(value.currentAddress());
        addreses[value.currentAddress()] = false;
        i++;
    }

    // //make conn devices unavailable
    // for (int i = 0; i < sensors.size(); i++) {
    //   usedAddreses.emplace_back(sensors.at(i).currentAddress());
    //   addreses[sensors.at(i).currentAddress()] = false;
    // }


    for (byte address = 0; address < 127; address++) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();

        if (error == 0 || error == 4) {
            if (addreses[address]) {
                addSensor(address, &sensors, conflict);

            } else if (address != SCREEN_I2C_ADDRESS) {
                collUnAddresses.emplace_back(address);
            }
        }
    }

    if (usedAddreses.size() != collUnAddresses.size()) {
        for (int i = 0; i < usedAddreses.size(); i++) {
            bool exists = false;
            for (int j = 0; j < collUnAddresses.size(); j++) {
                if (usedAddreses[i] == collUnAddresses[j]) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                sensors.erase(sensors.begin() + i);
            }
        }
    }
}
