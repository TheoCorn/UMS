//
// Created by theo on 7/11/2021.
//

#ifndef UMDWITHCLASS3_SYSINFO_H
#define UMDWITHCLASS3_SYSINFO_H

#include "SerialCom.h"

namespace sysInfo {

    struct BatteryInfo{
        String name;
        unsigned int capacity;
    };

    extern unsigned int screenAddress;
    extern String sn;
    extern BatteryInfo batteryInfo;
    extern SerialCom* serialCom;
    size_t serialComIndex;
    extern unsigned int batteryPercentage;

    extern String comName;

    extern bool isCharging;
}

#endif //UMDWITHCLASS3_SYSINFO_H
