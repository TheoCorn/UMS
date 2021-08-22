//
// Created by theo on 8/7/2021.
//

#ifndef UMDWITHCLASS3_SENSORSTAB_H
#define UMDWITHCLASS3_SENSORSTAB_H

#include "ui.h"
#include <map>
#include "Sensor.hpp"


class ui::SensorsTab : public ui::Tab {
private:

    class AllSensorsScreen;

    class SpecificSensorScreen;


    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &&activeSensorIterator;
    std::map<uint32_t, Sensor *> *sensors;

    ui::Tab *subScreen;

public:
    explicit SensorsTab(std::map<uint32_t, Sensor *> *sensors) : activeSensorIterator(sensors->begin()),
    sensors(sensors), subScreen(dynamic_cast<ui::Tab*>(new SensorsTab::AllSensorsScreen(activeSensorIterator, sensors, subScreen))) {}

    ~SensorsTab() {
         delete subScreen;
    }

    String name() override { return "SENSORS"; }

    void render(Adafruit_SSD1306 *display, coordinates &start, coordinates &end) override {

        display->setTextSize(1);
        display->setCursor(start.x, start.y);

        subScreen->render(display, start, end);
    }

    bool onUp() override { return subScreen->onUp(); }

    void onDown() override { SensorsTab::subScreen->onDown(); }

    void onClick() override { subScreen->onClick(); }


};


class AllSensorsScreen : public ui::Tab {
protected:
    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &activeSensorIterator;
    std::map<uint32_t, Sensor *> *sensors;
    ui::Tab *&subScreen;

    static void printSensors(Adafruit_SSD1306 *display,
                      std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &itStart,
                      std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &&itEnd,
                      ui::coordinates &end);

public:

    AllSensorsScreen(std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &activeSensorIterator,
                     std::map<uint32_t, Sensor *> *sensors,
                     ui::Tab *&subScreen) :
                     activeSensorIterator(activeSensorIterator), sensors(sensors), subScreen(subScreen) {}

    String name() override { return "SENSORS"; }

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

    void onClick() override;

    bool onUp() override;

    void onDown() override;

};


class SpecificSensor : public ui::Tab {
    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &activeSensorIterator;
    ui::Tab *&subScreen;
    std::map<uint32_t, Sensor *> *sensors;
public:

    SpecificSensor(std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &activeSensorIterator,
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


#endif //UMDWITHCLASS3_SENSORSTAB_H
