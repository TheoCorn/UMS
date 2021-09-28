//
// Created by theo on 8/7/2021.
//

#include "SensorsTab.h"


//void SensorsTab::switchSubScreen(SensorsTabSubScreen subScreen) {
//
//}

SensorsTab::SensorsTab(std::map<uint32_t, Sensor *> *sensors) : activeSensorIterator(sensors->begin()),
                                                                sensors(sensors), subScreen(dynamic_cast<ui::Tab*>(new AllSensorsScreen(sensors, activeSensorIterator))) {
//        subScreen = dynamic_cast<ui::Tab*>(new AllSensorsScreen(activeSensorIterator, sensors, subScreen));
}

void SensorsTab::onClick() {
    if(subScreenType == SensorsTabSubScreen::ALL_SENSORS_SCREEN){
        subScreenType = SensorsTabSubScreen::SPECIFIC_SENSOR_SCREEN;
        delete subScreen;
        subScreen = dynamic_cast<ui::Tab*>(new SpecificSensorScreen(activeSensorIterator->first, sensors));
    }else{
        subScreenType = SensorsTabSubScreen::ALL_SENSORS_SCREEN;
        delete subScreen;
        subScreen = dynamic_cast<ui::Tab*>(new AllSensorsScreen(sensors, activeSensorIterator));
    }
}
