//
// Created by theo on 9/14/2021.
//

#include "SettingsTab.h"
#include "Setting.h"

//SettingsTab::SettingsTab() {
////    std::vector<const char *> set SERIAL_COMM_NAMES;
//    std::vector<const char *> set {"BT_SPP", "USB"};
//    GeneralSetting setting = GeneralSetting("prot", set, 0);
////    settingsArr[0] = GeneralSetting("prot", set, sysInfo::serialComIndex);
//settingsArr[0] = setting;
//}

void SettingsTab::render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end){

    display->setCursor(start.x, start.y);
    if (isConfigSetting) {
        display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    }

    size_t xOffset = (start.x - end.x)/2 + start.x - 3;

    printSetting(display, settArrIndex, xOffset);

    display->setTextColor(SSD1306_WHITE);

    for (int i = settArrIndex + 1; i < SETTINGS_ARRAY_SIZE; ++i) {
        printSetting(display, i, xOffset);
    }
}

void SettingsTab::printSetting(Adafruit_SSD1306 *display, size_t index, size_t xOffset) {
    GeneralSetting& setting = *settingsArr[index];
    display->print(setting.name);
    const int y = display->getCursorY();
//    display->setCursor(xOffset, y);
    display->print(": ");
    display->println(setting.values[setting.currentIndex]);
}

bool SettingsTab::onUp() {
    if (isConfigSetting){
        (*settingsArr[settArrIndex]).onUp();
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
        (*settingsArr[settArrIndex]).onDown();
        return;
    }else{
        if (settArrIndex + 1 < SETTINGS_ARRAY_SIZE) settArrIndex++;
    }
}

void SettingsTab::onClick() {
    if (isConfigSetting){

        isConfigSetting = false;
//        auto& fun = (*settingsArr[settArrIndex]).onSet;
//        auto index = (*settingsArr[settArrIndex]).currentIndex;
//        fun(index);

        settingsArr[settArrIndex]->set();

    }else{
        isConfigSetting = true;
    }
}
