//
// Created by theo on 8/22/2021.
//

#include "AllSensorsScreen.h"

void AllSensorsScreen::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) {
    display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);

    std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> it = activeSensorIterator;

    String s = activeSensorIterator->second->getStringForDisplay();
    display->println(s);
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
//        printf("coordinates end x: %d, y%; current cursor y: %d", end.x, end.y, display->getCursorY());
        if (display->getCursorY() >= end.y) return;
        display->println(itStart->second->getStringForDisplay());
    }
}
