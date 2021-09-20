//
// Created by theo on 9/14/2021.
//

#include "SysInfoTab.h"


void SysInfoTab::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end){
    display->print("sn: ");
    display->println(sysInfo::sn);

    display->print("protocol: ");
    display->println(sysInfo::comName);

    display->print("display address: ");
    display->println(sysInfo::screenAddress);

    display->print("bat name: ");
    display->println(sysInfo::batteryInfo::name);

    display->print("bat capacity: ");
    display->print(sysInfo::batteryInfo.capacity);
}
