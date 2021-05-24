//
// Created by theo on 4/24/2021.
//

#ifndef UMDWITHCLASS3_FINDSENSORS_H
#define UMDWITHCLASS3_FINDSENSORS_H

#include <vector>
#include <map>
#include "ConflictingSensors.hpp"
#include "AddressIdentifier.hpp"

namespace ss{
    void checkI2C(std::vector<csa::ConflictingAddressStruct*> * conflict, std::map<uint8_t, Sensor*> * sensors, SensorsIdentifierManager * sim);
}




#endif //UMDWITHCLASS3_FINDSENSORS_H
