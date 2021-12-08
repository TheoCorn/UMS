//
// Created by theo on 8/22/2021.
//

#include "AllSensorsScreen.h"

void AllSensorsScreen::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) {

    if (!sensors->empty()){

        //checks if active sensor iterator is a valid iterator
        try {
            auto rsid = activeSensorIterator->first;
        } catch (...) {
            activeSensorIterator = sensors->begin();
            Serial.println("active sensor iterator invalid");
        }

        Serial.println("after try catch");

        display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);

        std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> it = activeSensorIterator;
        Serial.println("after iterator is set");

        display->println(activeSensorIterator->second->getStringForDisplay());

        Serial.println("after printing first sensor");

        printSensors(display, it, sensors->end(), end);
        Serial.println("after printing other sensors");
    }

}

bool AllSensorsScreen::onUp() {

    if (activeSensorIterator != sensors->begin()) {
        activeSensorIterator--;
        return false;
    }

    return true;

}

void AllSensorsScreen::onDown() {
    auto end = sensors->end();
    end--;
    if (activeSensorIterator != end) activeSensorIterator++;

}

void AllSensorsScreen::onClick() {
//    subScreen = dynamic_cast<ui::Tab*>(new class SpecificSensorScreen(activeSensorIterator, subScreen, sensors));
//    delete this;
}

void AllSensorsScreen::printSensors(Adafruit_SSD1306 *display,
                                    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &itStart,
                                    std::_Rb_tree_iterator <std::pair<const uint32_t, Sensor *>> &&itEnd,
                                    ui::coordinates &end) {

    display->setTextColor(SSD1306_WHITE);
    
    for (++itStart; itStart != itEnd; ++itStart) {
//        Serial.printf("coordinates end x: %d, y: %d; current cursor y: %d\n", end.x, end.y, display->getCursorY());
//        if ( end.y <= display->getCursorY() ) return;
//        Serial.println()
        display->println(itStart->second->getStringForDisplay());
    }
}
