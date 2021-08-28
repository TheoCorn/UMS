//
// Created by theo on 8/22/2021.
//

#ifndef UMDWITHCLASS3_SPECIFICSENSORSCREEN_H
#define UMDWITHCLASS3_SPECIFICSENSORSCREEN_H

#include "ui.h"
#include <map>
#include "Sensor.hpp"
#include "AllSensorsScreen.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class SpecificSensorScreen : public ui::Tab {

    std::map<uint32_t, Sensor *> *sensors;
    std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> activeSensorIterator;

public:

    SpecificSensorScreen(std::map<uint32_t, Sensor *> *sensors) : sensors(sensors) {
        if (!sensors->empty()) {
            activeSensorIterator = sensors->begin();
            String name = activeSensorIterator->second->name();

            Serial.println(name);
        }else{
            Serial.println("sensors empty");
        }
    }

    String name() override { return "SENSOR DATA"; }

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

    bool onUp() override { return false; };

    void onClick() override;
};


#endif //UMDWITHCLASS3_SPECIFICSENSORSCREEN_H
