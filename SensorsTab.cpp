//
// Created by theo on 8/7/2021.
//

#include "SensorsTab.h"


void AllSensorsScreen::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) {
    display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);

    auto it = activeSensorIterator;


    display->println(it->second->getStringForDisplay());
    printSensors(display, ++it, sensors->end(), end);

}

bool AllSensorsScreen::onUp() {
    if (activeSensorIterator != sensors->begin()) {
        activeSensorIterator--;
        return false;
    } else {
        return true;
    }
}

void AllSensorsScreen::onDown() {
    if (activeSensorIterator != sensors->end()) activeSensorIterator++;
}

void AllSensorsScreen::onClick() {
    subScreen = (ui::Tab *) (new SpecificSensor(activeSensorIterator, subScreen, sensors));
    delete this;
}

void AllSensorsScreen::printSensors(Adafruit_SSD1306 *display,
                                                    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &itStart,
                                                    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &&itEnd,
                                                    ui::coordinates &end) {

    display->setTextColor(SSD1306_WHITE);

    for (itStart; itStart != itEnd; ++itStart) {
        if (display->getCursorY() >= end.y) return;
        display->println(itStart->second->getStringForDisplay());
    }
}


void SpecificSensor::render(Adafruit_SSD1306 *display,
                                            ui::coordinates &start,
                                            ui::coordinates &end) {
    display->println(activeSensorIterator->second->getExtendedStringForDisplay());
}


void SpecificSensor::onClick() {
    subScreen = (ui::Tab *) (new AllSensorsScreen(activeSensorIterator, sensors, subScreen));
    delete this;
}
