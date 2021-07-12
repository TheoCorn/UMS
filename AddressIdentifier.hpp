

#ifndef AddressIdentiofier_h
#define AddressIdentiofier_h

#include <vector>
#include "Sensor.hpp"
#include "ConflictingSensors.hpp"
#include "sensorEnum.h"
#include "JsonParserFunctions.hpp"



/**
 * this class identifies Sensors based on the i2c address it creates
 *
 *
 * @param
 * @return
 */
class SensorsIdentifierManager {
  public:

    SensorsIdentifierManager(){
        init();
    }

    ~SensorsIdentifierManager(){
        delete[] numEnumSensorInVectorArray;
    }

    void addSensor(uint8_t address, std::map<uint8_t, Sensor*> * sensors, std::vector<csa::ConflictingAddressStruct*> * conflict);
    void addSensor(unsigned int enumPos, uint8_t address, std::map<uint8_t, Sensor*> * sensors, std::vector<csa::ConflictingAddressStruct*> * conflict);

  private:
    std::vector<unsigned int> * numEnumSensorInVectorArray;
    void init();
    void JsonObjectToArrOfVectors(JsonDocument* doc);


    static Sensor *getSensorPointerForEnumPos(unsigned int enumPos, uint8_t address);
};


#endif
