//
// Created by theo on 9/14/2021.
//

#ifndef UMDWITHCLASS3_SETTINGSTAB_H
#define UMDWITHCLASS3_SETTINGSTAB_H

#include "ui.h"
#include "sysInfo.h"
#include "SerialCom.h"
#include "Setting.h"

#define SETTINGS_ARRAY_SIZE 1

class SettingsTab : public ui::Tab {


    /**
     * array that holds all system settings
     *
     * @see Setting
     */
    GeneralSetting settingsArr[SETTINGS_ARRAY_SIZE] {
        GeneralSetting("prot", std::vector<const char *> {"BT_SPP", "USB"}, 0)
    };

    size_t settArrIndex = 0;

    ///variable that describes whether you are iterating @settingArr or the individual Settings
    bool isConfigSetting = false;

    void printSetting(Adafruit_SSD1306 *display, size_t index, size_t xOffset);


public:

//    SettingsTab(){
//        std::vector<const char *> set {"BT_SPP", "USB"};
//        GeneralSetting setting = GeneralSetting("prot", set, 0);
//        settingsArr[0] = setting;
//    }

    String name() override { return "SETTINGS"; }

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

    bool onUp() override;

    void onDown() override;

    void onClick() override;

};


#endif //UMDWITHCLASS3_SETTINGSTAB_H
