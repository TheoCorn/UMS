//
// Created by theo on 4/24/2021.
//

#ifndef UMDWITHCLASS3_FINDSENSORS_H
#define UMDWITHCLASS3_FINDSENSORS_H

#include <vector>
#include <map>
#include "ConflictingSensors.hpp"
#include "AddressIdentifier.hpp"

/**
 * sensor search.
 * This is where all future checkers will go.
 * @defgroup sensor_search
 * @{
 */
namespace ss{

    /**
     * search and connect i2c sensors
     */
    void checkI2C(std::vector<csa::ConflictingAddressStruct*> * conflict, std::map<uint32_t , Sensor*> * sensors, SensorsIdentifierManager * sim);
}

/** @} */


#endif //UMDWITHCLASS3_FINDSENSORS_H
