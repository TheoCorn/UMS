//
// Created by theo on 8/22/2021.
//

#include "SpecificSensorScreen.h"


void SpecificSensorScreen::render(Adafruit_SSD1306 *display,
                            ui::coordinates &start,
                            ui::coordinates &end) {

    try{
        // todo fix nonsense code, crashes when direct because of WString
        // I dont know what I meant by this to do but cant find the bug anymore
        String str = sensors->at(rsid)->getExtendedStringForDisplay();
        display->println(str);
    } catch (std::out_of_range&) {
        display->println("sensor not available");
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
    } catch (std::out_of_range&) {
        return "";
    }
}
