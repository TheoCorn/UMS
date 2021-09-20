//
// Created by theo on 9/14/2021.
//

#include "SysInfoTab.h"


void SysInfoTab::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end){

    display->setCursor(start.x, start.y);

    display->print("sn: ");
    display->println(sysInfo::sn);

    display->print("prot: ");
    display->print(sysInfo::comName);

    int y = display->getCurosorY();
    display->setCursor((end.x - start.x) /2 + start.x);
    display->print("DA: ");
    display->println(sysInfo::screenAddress);

    display->print("bat name: ");
    display->println(sysInfo::batteryInfo.name);

    display->print("bat capacity: ");
    display->print(sysInfo::batteryInfo.capacity);
}
