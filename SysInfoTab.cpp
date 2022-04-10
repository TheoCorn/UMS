//
// Created by theo on 9/14/2021.
//

#include "SysInfoTab.h"


void SysInfoTab::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end){

    display->setCursor(start.x, start.y);

    display->print("sn: ");
    display->println(sysInfo::sn);

    display->print("name: ");
    display->println(sysInfo::comName);


    display->print("DA: ");
    display->print(sysInfo::screenAddress);

    const int y = display->getCursorY();
    display->setCursor((end.x - start.x) /2 + start.x, y);

    display->print("bn: ");
    display->println(sysInfo::batteryInfo.name);

    display->print("bat capacity: ");
    display->print(sysInfo::batteryInfo.capacity);

}
