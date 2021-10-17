//
// Created by theo on 9/21/2021.
//

#include "sTabOnSet.h"
#include "gpioNames.h"
//#include "sysInfo.h"

void sTabOnSet::comChange(unsigned int index){
    delay(1000);
    delete sysInfo::serialCom;

    sysInfo::serialCom = getSerialCom4EnumPos(index);
}

void sTabOnSet::sysChange(unsigned int index){
    if (index == 1){
//        sysInfo::mDisplay->sleep();

        digitalWrite(SCREEN_EN_PIN, LOW);
        Wire.~TwoWire();
        pinMode(21, OUTPUT);
        pinMode(22, OUTPUT);

        digitalWrite(21, LOW);
        digitalWrite(22, LOW);

        gpio_hold_en(GPIO_NUM_5);
        gpio_hold_en(GPIO_NUM_21);
        gpio_hold_en(GPIO_NUM_22);

        esp_deep_sleep_start();
    }
}
