//
// Created by theo on 7/11/2021.
//

#ifndef UMDWITHCLASS3_SYSINFO_H
#define UMDWITHCLASS3_SYSINFO_H

#include "SerialCom.h"
#include "ArduinoJson.h"
//#include "displayFunctions.h"

namespace sysInfo {

    struct BatteryInfo{
        String name;
        unsigned int capacity;
    };

    extern unsigned int screenAddress;
    extern bool hasScreen;
    extern String sn;
    extern BatteryInfo batteryInfo;
    extern SerialCom* serialCom;
    extern size_t serialComIndex;
    extern unsigned int batteryPercentage;

    extern String comName;

//    DisplayFunctions *mDisplay;

    extern bool isCharging;

//    JsonDocument* getSysInfo(){
//        auto *doc = new DynamicJsonDocument(2048);
//        auto sio = doc->createNestedObject("SysInfo");
//        sio["sn"] = sn;
//        sio["batteryPercentage"] = batteryPercentage;
//        sio["isCharging"] = isCharging;
//        sio["screenAddress"] = screenAddress;
//
//        return doc;
//    }
}

#endif //UMDWITHCLASS3_SYSINFO_H
