

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

    void addSensor(byte address, std::map<uint8_t, Sensor*> * sensors, std::vector<csa::ConflictingAddressStruct> * conflict);
    void addSensor(unsigned int enumPos, byte address, std::map<uint8_t, Sensor*> * sensors);

  private:
    std::vector<unsigned int> * numEnumSensorInVectorArray;
    void init();
    void JsonObjectToArrOfVectors(JsonObject* obj, void (*actaulDo)(JsonPair*));


    static Sensor *getSensorPointerForEnumPos(unsigned int enumPos, byte address);
};


#endif
