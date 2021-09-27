//
// Created by theo on 9/27/2021.
//

#ifndef UMDWITHCLASS3_JSONCOMMANDSFUNCTIONS_H
#define UMDWITHCLASS3_JSONCOMMANDSFUNCTIONS_H

#include <ArduinoJson.h>
#include "JsonParserFunctions.hpp"
#include "JsonSerializer.h"
#include "sysInfo.h"
#include "err.h"

namespace jcf{
    void onSensorsElementReceive(JsonVariant *v);

    void onGetElementReceive(JsonVariant *v);
}


#endif //UMDWITHCLASS3_JSONCOMMANDSFUNCTIONS_H
