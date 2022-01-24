//
// Created by theo on 8/22/2021.
//

#include "AllSensorsScreen.h"

void AllSensorsScreen::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) {

    //todo fix bug when first sensor is being connected sys restart then works fine

    if (!sensors->empty()){

        //checks if active sensor iterator is a valid iterator
        try {
            auto rsid = activeSensorIterator->first;

            display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);

            std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> it = activeSensorIterator;

            display->println(activeSensorIterator->second->getStringForDisplay());

//            Serial.println("after iterator used");

            printSensors(display, it, sensors->end(), end);
        } catch (...) {
            activeSensorIterator = sensors->begin();
//            Serial.println("catch");
        }

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
    if (sensors->empty()) return;
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
