//
// Created by theo on 8/22/2021.
//

#ifndef UMDWITHCLASS3_SPECIFICSENSORSCREEN_H
#define UMDWITHCLASS3_SPECIFICSENSORSCREEN_H

#include "ui.h"
#include <map>
#include "Sensor.hpp"
#include "AllSensorsScreen.h"
#include "SensorsTab.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class SpecificSensorScreen : public ui::Tab {
    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &activeSensorIterator;
    ui::Tab *&subScreen;
    std::map<uint32_t, Sensor *> *sensors;
public:

    SpecificSensorScreen(std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &activeSensorIterator,
                         ui::Tab *&subScreen,
                         std::map<uint32_t, Sensor *> *sensors) :

                   activeSensorIterator(activeSensorIterator),
                   subScreen(subScreen),
                   sensors(sensors) {}

                   String name() override { return activeSensorIterator->second->name(); }

                   void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

    bool onUp() override { return false; };

    void onClick() override;
};


#endif //UMDWITHCLASS3_SPECIFICSENSORSCREEN_H
