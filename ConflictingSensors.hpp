
#ifndef ConflictingSensor_h
#define ConflictingSensor_h

#include "Sensor.hpp"

//conflicting sensor address
namespace csa{

typedef struct {
  std::vector<String> nameOfSensors;
  std::vector<unsigned int> EnumPosOfSensors;
  byte address;
  
} ConflictingAddressStruct;

}

#endif
