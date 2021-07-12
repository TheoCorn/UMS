//
// Created by theo on 7/11/2021.
//

#ifndef UMDWITHCLASS3_SYSINFO_H
#define UMDWITHCLASS3_SYSINFO_H

namespace sysInfo {

    struct BatteryInfo{
        String name;
        unsigned int capacity;
    };

    extern unsigned int screenAddress;
    extern String snid;
    extern BatteryInfo batteryInfo;
    extern SerialCom* serialCom;
    extern unsigned int batteryPercentage;
}

#endif //UMDWITHCLASS3_SYSINFO_H
