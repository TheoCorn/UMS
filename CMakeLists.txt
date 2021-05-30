cmake_minimum_required(VERSION 3.19)
project(UMDWithClass3)

set(CMAKE_CXX_STANDARD 14)

include_directories(. D:/ESP-IDF/esp-idf/components)

add_executable(UMDWithClass3
        AddressIdentifier.cpp
        AddressIdentifier.hpp
        BluetoothSerial.cpp
        BluetoothSerial.h
        ConflictingSensors.hpp
        displayFunctions.cpp
        displayFunctions.h
        findSensors.cpp
        findSensors.h
        JsonParserFunctions.cpp
        JsonParserFunctions.hpp
        MPU9250.cpp
        MPU9250.hpp
        Sensor.cpp
        Sensor.hpp
        sensorEnum.h
        spiffs.cpp
        spiffs.hpp
        UMDWithClass3.ino
        gpioNames.h ConflictingSensors.cpp)
