//
// Created by theo on 8/22/2021.
//

#include "SpecificSensorScreen.h"


void SpecificSensor::render(Adafruit_SSD1306 *display,
                            ui::coordinates &start,
                            ui::coordinates &end) {
    display->println(activeSensorIterator->second->getExtendedStringForDisplay());
}


void SpecificSensor::onClick() {
    subScreen = (ui::Tab *) (new AllSensorsScreen(activeSensorIterator, sensors, subScreen));
    delete this;
}