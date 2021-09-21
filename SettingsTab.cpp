//
// Created by theo on 9/14/2021.
//

#include "SettingsTab.h"

SettingsTab::SettingsTab() {
//    std::vector<const char *> set SERIAL_COMM_NAMES;
//    Setting setting = Setting("prot", set, sysInfo::serialComIndex);
//    settingsArr[0] = Setting("prot", set, sysInfo::serialComIndex);
}

void SettingsTab::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end){

    display->setCursor(start.x, start.y);
    if (isConfigSetting) {
        display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    }

    size_t xOffset = (start.x - end.x)/2 + start.x - 3;

    printSetting(settArrIndex, xOffset);

    display->setTextColor(SSD1306_WHITE);

    for (int i = settArrIndex + 1; i < SETTINGS_ARRAY_SIZE; ++i) {
        printSetting(i, xOffset);
    }
}

void SettingsTab::printSetting(size_t index, size_t xOffset) {
    Setting& setting = settingsArr[index];
    display->print(setting.name);
    const int y = display->getCursorY();
    display->setCursor(xOffset, y);
    display->print(": ");
    display->println(setting.values[setting.currentIndex]);
}

bool SettingsTab::onUp() {
    if (isConfigSetting){
        settingsArr[settArrIndex].onUp();
        return false;
    }else{
        if (settArrIndex == 0){
            return true;
        }else{
            settArrIndex--;
            return false;
        }
    }
}

void SettingsTab::onDown() {
    if (isConfigSetting){
        settingsArr[settArrIndex].onDown();
        return;
    }else{
        if (settArrIndex + 1 < settingsArr.size()) settArrIndex++;
    }
}

void SettingsTab::onClick() {
    isConfigSetting = !isConfigSetting;
}
