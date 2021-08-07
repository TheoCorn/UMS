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

    std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &&activeSensorIterator;
    std::map<unsigned int, Sensor *> *sensors;

    ui::Tab *subScreen;

public:
    explicit SensorsTab(std::map<unsigned int, Sensor *> *sensors) : sensors(sensors),
                                                                     activeSensorIterator(sensors->begin()) {}

    void render(Adafruit_SSD1306 *display, coordinates &start, coordinates &end) override {

        display->setTextSize(1);
        display->setCursor(start.x, start.y);

        subScreen->render(display, start, end)
    }

    bool onUp() override { subScreen->onUp() }

    void onDown() override { SensorsTab::subScreen->onDown() }

    void onClick() override { subScreen->onClick() }


};


class ui::AllSensorsScreen : public ui::Tab {
protected:
    std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &activeSensorIterator;
    std::map<unsigned int, Sensor *> *sensors;
    ui::Tab *&subScreen;

    void printSensors(Adafruit_SSD1306 *display,
                      std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &itStart,
                      std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &&itEnd,
                      ui::coordinates &end);

public:

    AllSensorsScreen(std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &activeSensorIterator,
                     std::map<unsigned int, Sensor *> *sensors,
                     ui::Tab *&subScreen) :
            sensors(sensors), activeSensorIterator(activeSensorIterator), subScreen(subScreen) {}

    void render(Adafruit_SSD1306 *display, coordinates &start, coordinates &end) override;

    void onClick() override;

};


class ui::SpecificSensor : public ui::Tab {
    std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &activeSensorIterator;
    ui::Tab *&subScreen;
    std::map<unsigned int, Sensor *> *sensors;
public:

    SpecificSensor(std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &activeSensorIterator,
                   ui::Tab *&subScreen,
                   std::map<unsigned int, Sensor *> *sensors) :

            activeSensorIterator(activeSensorIterator),
            subScreen(subScreen),
            sensors(sensors) {}

    void render(Adafruit_SSD1306 *display, coordinates &start, coordinates &end) override;

    bool onUp() override { return false; };

    void onClick() override;
};


#endif //UMDWITHCLASS3_SENSORSTAB_H
