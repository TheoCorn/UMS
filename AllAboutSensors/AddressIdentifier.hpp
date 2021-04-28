

#ifndef AddressIdentiofier_h
#define AddressIdentiofier_h

#include <vector>
#include "Sensor.hpp"
#include "ConflictingSensors.hpp"
#include "sensorEnum.h"
#include "JsonParserFunctions.hpp"




class SensorsIdentifierManager {
  public:

    SensorsIdentifierManager(){
        init();
    }

    ~SensorsIdentifierManager(){
        delete numEnumSensorInVectorArray;
    }

    void addSensor(byte address, std::vector<Sensor> * vector, std::vector<csa::ConflictingAddressStruct> * conflict);


  private:
    std::vector<unsigned int> * numEnumSensorInVectorArray;
    void init();
    void JsonObjectToArrOfVectors(JsonObject* obj, void (*actaulDo)(JsonPair*));



};


#endif
