
#ifndef ConflictingSensor_h
#define ConflictingSensor_h

#include "Sensor.hpp"
#include <utility>
#include <vector>
#include "json_key_words.h"

//conflicting sensor address
namespace csa{

struct ConflictingAddressStruct {
//  std::vector<String> nameOfSensors;
//  std::vector<unsigned int> EnumPosOfSensors;
//  uint8_t address;

    uint32_t rsid;
    std::vector<unsigned int> sids;

    ConflictingAddressStruct(uint32_t rsid, std::vector<unsigned int> sids) : rsid(rsid), sids(std::move(sids)) {
        sids.shrink_to_fit();
    }
  
};

    JsonDocument* conflictsToJsonDoc(std::vector<csa::ConflictingAddressStruct*>* conflicts);

}

#endif
