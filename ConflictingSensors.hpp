
#ifndef ConflictingSensor_h
#define ConflictingSensor_h

#include "Sensor.hpp"

//conflicting sensor address
namespace csa{

typedef struct {
  std::vector<Sensor> confSensors;
  byte address;
  
} ConflictingAddressStruct;

}

#endif
