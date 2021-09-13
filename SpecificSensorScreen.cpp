//
// Created by theo on 8/22/2021.
//

#include "SpecificSensorScreen.h"


void SpecificSensorScreen::render(Adafruit_SSD1306 *display,
                            ui::coordinates &start,
                            ui::coordinates &end) {
    char const * str = activeSensorIterator->second->getExtendedStringForDisplay();
    Serial.printf(str);
    display->println(str);

    delete[] str;
//    display->println(sensors->begin()->second->getExtendedStringForDisplay());
}


void SpecificSensorScreen::onClick() {
//    subScreen = dynamic_cast<ui::Tab*>(new AllSensorsScreen(activeSensorIterator, sensors, subScreen));
//    delete this;
}

String SpecificSensorScreen::name() {
    if (sensors->empty()) return "SENSOR DATA";
    return activeSensorIterator->second->name();
}
