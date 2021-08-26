//
// Created by theo on 8/22/2021.
//

#ifndef UMDWITHCLASS3_ALLSENSORSSCREEN_H
#define UMDWITHCLASS3_ALLSENSORSSCREEN_H

#include "ui.h"
#include <map>
#include "Sensor.hpp"
#include "SensorsTab.h"
#include "SpecificSensorScreen.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class AllSensorsScreen : public ui::Tab {
protected:
    std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> &activeSensorIterator;
    std::map<uint32_t, Sensor *> *sensors;
    ui::Tab *&subScreen;

    static void printSensors(Adafruit_SSD1306 *display,
                             std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> &itStart,
                             std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> &&itEnd,
                             ui::coordinates &end);

public:

    AllSensorsScreen(std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> &activeSensorIterator,
                     std::map<uint32_t, Sensor *> *sensors,
                     ui::Tab *&subScreen) :
            activeSensorIterator(activeSensorIterator), sensors(sensors), subScreen(subScreen) {
        activeSensorIterator-
    }

    String name() override { return "SENSORS"; }


    void onClick() override;

    bool onUp() override;

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

    void onDown() override;


};


#endif //UMDWITHCLASS3_ALLSENSORSSCREEN_H
