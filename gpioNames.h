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
#define BATTERY_READ_PIN  13
#define BATTERY_IS_CHARGING 14

#define BUTTON_PIN      0

///rotary encoder pin A
#define REA             4

///rotary encoder pin B
#define REB             2
//
//#endif




#endif //UMDWITHCLASS3_GPIONAMES_H
