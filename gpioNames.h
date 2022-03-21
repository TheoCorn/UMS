//
// Created by theo on 5/15/2021.
//

#ifndef UMDWITHCLASS3_GPIONAMES_H
#define UMDWITHCLASS3_GPIONAMES_H

//different gpios for breadboard version
//#define ON_BREADBOARD_WEMOS_LOLIN31 0
//
//#ifdef ON_BREADBOARD_WEMOS_LOLIN32

///if changed change in sTabOnSet::sysChange
//#define SCREEN_EN_PIN       5
//
//#define BATTERY_READ_PIN  13
//#define BATTERY_IS_CHARGING 14
//
//#define BUTTON_PIN      19
//
/////rotary encoder pin A
//#define REA             34
//
/////rotary encoder pin B
//#define REB             35

//#else
//
///if changed change in sTabOnSet::sysChange
#define SCREEN_EN_PIN       5
#define BATTERY_READ_PIN  33
#define BATTERY_IS_CHARGING 16

#define BUTTON_PIN      0

///rotary encoder pin A
#define REA             4

///rotary encoder pin B
#define REB             2

//
//#endif

namespace ioPins {
/**
 * GPIO1 is connected to 3v3 port as Signal
 * it can be used as a ADC, DAC or standard digital
 * (most often used as a ADC)
 * not yet implemented
 */
    constexpr uint32_t v3Pin = 26;

/**
* itis connected to 5v port as Signal
* it can be used as a ADC, DAC or standard digital
* (most often used as a ADC)
* not yet implemented
 */
    constexpr uint32_t v5Pin = 25;
}


#endif //UMDWITHCLASS3_GPIONAMES_H
