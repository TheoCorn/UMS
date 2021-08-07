//
// Created by theo on 8/7/2021.
//

#include "SensorsTab.h"


void ui::SensorsTab::AllSensorsScreen::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) {
    display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);

    auto it = activeSensorIterator;


    display->println(it->second->getStringForDisplay());
    printSensors(display, ++it, sensors->end(), end);
    return;

}

bool ui::SensorsTab::AllSensorsScreen::onUp() {
    if (activeSensorIterator != sensors->begin()) {
        activeSensorIterator--;
        return false;
    } else {
        return true;
    }
}

void ui::SensorsTab::AllSensorsScreen::onDown() {
    if (activeSensorIterator != sensors->end()) activeSensorIterator++;
}

void ui::SensorsTab::AllSensorsScreen::onClick() {
    subScreen = (ui::Tab *) (new ui::SensorsTab::SpecificSensor(activeSensorIterator, subScreen, sensors));
    delete this;
}

void ui::SensorsTab::AllSensorsScreen::printSensors(Adafruit_SSD1306 *display,
                                                    std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &itStart,
                                                    std::_Rb_tree_iterator <std::pair<const unsigned int, Sensor *>> &&itEnd,
                                                    ui::coordinates &end) {

    display->setTextColor(SSD1306_WHITE);

    for (itStart; itStart != itEnd; ++itStart) {
        if (display->getCursorY >= end.y) return;
        display->println(itStart->second->getStringForDisplay());
    }
}


void ui::SensorsTab::SpecificSensor::render(Adafruit_SSD1306 *display,
                                            ui::coordinates &start,
                                            ui::coordinates &end) {
    display->println(activeSensorIterator->second->getExtendedStringForDisplay());
}

bool ui::SensorsTab::SpecificSensor::onUp() {
    return Tab::onUp();
}

void ui::SensorsTab::SpecificSensor::onClick() {
    subScreen = (ui::Tab *) (new ui::SensorsTab::AllSensorsScreen(activeSensorIterator, sensors, subScreen));
    delete this;
}
