//
// Created by theo on 4/24/2021.
//

#ifndef UMDWITHCLASS3_DISPLAYFUNCTIONS_H
#define UMDWITHCLASS3_DISPLAYFUNCTIONS_H

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_I2C_ADDRESS 0x3C

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <map>
#include <vector>
#include <functional>
#include "Sensor.hpp"
#include "SerialCom.h"
#include "sysInfo.h"
#include "SensorsTab.h"
#include "gpioNames.h"
#include "ui.h"

#include "AllSensorsScreen.h"
#include "SpecificSensorScreen.h"
#include "SysInfoTab.h"
#include "SettingsTab.h"

class DisplayFunctions {

private:
    Adafruit_SSD1306 * display;
    std::map<uint32_t, Sensor*> * sensors;

    ui::coordinates start = ui::coordinates(0, 17);
    ui::coordinates end = ui::coordinates(128, 64);

    std::vector<ui::Tab*> tabs {
        dynamic_cast<ui::Tab*>(new SensorsTab(sensors)),
        dynamic_cast<ui::Tab*>(new SettingsTab()),
        dynamic_cast<ui::Tab*>(new SysInfoTab())
        };

//    std::vector<ui::Tab*> tabs { dynamic_cast<ui::Tab*>( new AllSensorsScreen(sensors) ), dynamic_cast<ui::Tab*>(new SpecificSensorScreen(sensors)) };

    std::vector<ui::Tab*>::iterator tabIterator;
    bool iteratingTabs = false;

    void reStatusDispatch();


public:

    bool wasClicked = false;
    bool reaWasLow = false;
    bool rebWasLow = false;


    explicit DisplayFunctions(std::map<uint32_t , Sensor *> *sensors): sensors(sensors), tabIterator(tabs.begin()) {

        display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

        init();

    }


    ~DisplayFunctions() = default;

    void onClick();

    void displayWhenReading();

    void displayWhenNotReading();

    void showReading(bool reading);

    void showBattery();

    void showComInfo();

    void sleep();

    void init();

};




#endif //UMDWITHCLASS3_DISPLAYFUNCTIONS_H
