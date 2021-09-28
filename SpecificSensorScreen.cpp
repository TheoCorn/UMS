//
// Created by theo on 8/22/2021.
//

#include "SpecificSensorScreen.h"


void SpecificSensorScreen::render(Adafruit_SSD1306 *display,
                            ui::coordinates &start,
                            ui::coordinates &end) {

    String str;

    try{
        str = sensors->at(rsid)->getExtendedStringForDisplay();
    } catch (std::out_of_range) {
        str = SENSOR_NOT_CONNECTED;
    }

}


void SpecificSensorScreen::onClick() {
//    subScreen = dynamic_cast<ui::Tab*>(new AllSensorsScreen(activeSensorIterator, sensors, subScreen));
//    delete this;
}

String SpecificSensorScreen::name() {

    try {
        String name = sensors->at(rsid)->name();
        return name;
    } catch (std::out_of_range) {
        return userMessages::NO_MESSAGE;
    }
}

