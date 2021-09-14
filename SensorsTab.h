//
// Created by theo on 8/7/2021.
//

#ifndef UMDWITHCLASS3_SENSORSTAB_H
#define UMDWITHCLASS3_SENSORSTAB_H

#include "ui.h"
#include <map>
#include "Sensor.hpp"
#include "AllSensorsScreen.h"
#include "SpecificSensorScreen.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


enum class SensorsTabSubScreen{
    ALL_SENSORS_SCREEN,
    SPECIFIC_SENSOR_SCREEN
};


class SensorsTab : public ui::Tab {
private:

    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> activeSensorIterator;
    std::map<uint32_t, Sensor *> *sensors;

    ui::Tab *subScreen;
    SensorsTabSubScreen subScreenType = SensorsTabSubScreen::ALL_SENSORS_SCREEN;

public:
    explicit SensorsTab(std::map<uint32_t, Sensor *> *sensors);

    ~SensorsTab() {
         delete subScreen;
    }

    String name() override { return "SENSORS"; }

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override {

        display->setTextSize(1);
        display->setCursor(start.x, start.y);

        subScreen->render(display, start, end);

    }

    bool onUp() override { return subScreen->onUp(); }

    void onDown() override { subScreen->onDown(); }

    void onClick() override;

//    void switchSubScreen(SensorsTabSubScreen subScreen);

};








#endif //UMDWITHCLASS3_SENSORSTAB_H
