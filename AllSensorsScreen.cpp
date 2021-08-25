//
// Created by theo on 8/22/2021.
//

#include "AllSensorsScreen.h"

void AllSensorsScreen::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) {
//    display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);

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
    subScreen = dynamic_cast<ui::Tab*>(new class SpecificSensorScreen(activeSensorIterator, subScreen, sensors));
    delete this;
}

void AllSensorsScreen::printSensors(Adafruit_SSD1306 *display,
                                    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &itStart,
                                    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &&itEnd,
                                    ui::coordinates &end) {

    display->setTextColor(SSD1306_WHITE);

    for (; itStart != itEnd; ++itStart) {
        if (display->getCursorY() >= end.y) return;
        display->println(itStart->second->getStringForDisplay());
    }
}
