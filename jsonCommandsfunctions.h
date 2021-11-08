//
// Created by theo on 9/27/2021.
//

#ifndef UMDWITHCLASS3_JSONCOMMANDSFUNCTIONS_H
#define UMDWITHCLASS3_JSONCOMMANDSFUNCTIONS_H

#include <ArduinoJson.h>
#include "JsonParserFunctions.hpp"
#include "JsonSerializer.h"
#include "sysInfo.h"
#include "json_key_words.h"
#include "Error.h"
#include "AddressIdentifier.hpp"


namespace jcf{
    void onSensorsElementReceive(JsonVariant *v, std::map<uint32_t, Sensor *> *sensors);

    void onGetElementReceive(JsonVariant *v, std::map<uint32_t, Sensor *> *sensors);

    void onClearConflict(JsonVariant *v, std::map<uint32_t,Sensor *> *sensors,
                         std::vector<csa::ConflictingAddressStruct *>& conflicts);
}


#endif //UMDWITHCLASS3_JSONCOMMANDSFUNCTIONS_H
