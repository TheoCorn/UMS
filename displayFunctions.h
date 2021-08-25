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


class DisplayFunctions {

private:
    Adafruit_SSD1306 * display;
    std::map<uint32_t, Sensor*> * sensors;

    ui::coordinates start = ui::coordinates(0, 17);
    ui::coordinates end = ui::coordinates(128, 64);

    std::vector<ui::Tab*> tabs { dynamic_cast<ui::Tab*>(new SensorsTab(sensors))};
    std::vector<ui::Tab*>::iterator tabIterator;

//    bool rePinALast = false;


public:
    explicit DisplayFunctions(std::map<uint32_t , Sensor *> *sensors): sensors(sensors) {

        display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
        tabIterator = tabs.begin();

        init();

        display->clearDisplay();
        display->println("hi");
        (*tabIterator)->render(display, start, end);
        display->display();

    }


    ~DisplayFunctions() = default;

    void onREAInterrupt();

    void onClick();

    void displayWhenReading();

    void displayWhenNotReading();

    void showReading(bool reading);

    void showBattery();

    void showSensors();

    void showComInfo();

    void sleep();

    void init();

//    namespace ui{
//        std::vector<String> tabNames{"Sensors", "Settings", "com"};
//        std::vector<std::function<void(Adafruit_SSD1306*)> uiRenderers{std::function<void()>(showSensors) };
//    }


};




#endif //UMDWITHCLASS3_DISPLAYFUNCTIONS_H
