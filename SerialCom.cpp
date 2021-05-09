//
// Created by theo on 5/9/2021.
//

#include "SerialCom.h"
#include <ArduinoJson.h>


bool SerialCom::begin(unsigned long boudRate) {
    return false;
}

int SerialCom::available() {
    return null;
}

bool SerialCom::read(char *c) {
    return false;
}

size_t SerialCom::write(const int) {
    return 0;
}

size_t SerialCom::write(const int *buffer, size_t) {
    return 0;
}

size_t SerialCom::write(const JsonDocument * doc){
    return 0;
}


void flush(){

}