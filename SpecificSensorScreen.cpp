//
// Created by theo on 8/22/2021.
//

#include "SpecificSensorScreen.h"


void SpecificSensorScreen::render(Adafruit_SSD1306 *display,
                            ui::coordinates &start,
                            ui::coordinates &end) {

    String str;

    try{
        // todo fix nonsense code, crashes when direct because of WString
        String mstr = sensors->at(rsid)->getExtendedStringForDisplay();
        str = mstr;
    } catch (std::out_of_range&) {
        str = "sensor not available";
    }

    display->println(str);

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
