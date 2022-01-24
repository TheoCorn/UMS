//
// Created by theo on 9/21/2021.
//

#include "sTabOnSet.h"
#include "gpioNames.h"
//#include "sysInfo.h"

void sTabOnSet::comChange(unsigned int index){
    delay(1000);
    delete sysInfo::serialCom;

    auto scom = getSerialCom4EnumPos(index);
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

//        *(IO_MUX_GPIO_21_REG) << ( 1ULL << 010 10 1 11 00 00 1 00 11 00

//        uint32_t gpio_setting = 0b0101011100001001100;
//        set_io_mux_gpio( (uint32_t*) IO_MUX_GPIO_21_REG, gpio_setting);
//        set_io_mux_gpio((uint32_t*) IO_MUX_GPIO_22_REG, gpio_setting);
//


        gpio_pulldown_en(GPIO_NUM_21);
        gpio_pulldown_en(GPIO_NUM_22);

        gpio_hold_en(GPIO_NUM_5);
        gpio_hold_en(GPIO_NUM_21);
        gpio_hold_en(GPIO_NUM_22);

        esp_deep_sleep_start();
    }
}
