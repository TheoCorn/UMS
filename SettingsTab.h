//
// Created by theo on 9/14/2021.
//

#ifndef UMDWITHCLASS3_SETTINGSTAB_H
#define UMDWITHCLASS3_SETTINGSTAB_H

#define SETTINGS_ARRAY_SIZE 1

#include "ui.h"
#include "sysInfo.h"
#include "SerialCom.h"
#include "Setting.h"



class SettingsTab : public ui::Tab {


    /**
     * array that holds all system settings
     *
     * @see Setting
     */
    GeneralSetting settingsArr[SETTINGS_ARRAY_SIZE];

    size_t settArrIndex = 0;

    ///variable that describes whether you are iterating @settingArr or the individual Settings
    bool isConfigSetting = false;

    void printSetting(Adafruit_SSD1306 *display, size_t index, size_t xOffset);


public:


    String name() override { return "SETTINGS"; }

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

    bool onUp() override;

    void onDown() override;

    void onClick() override;

};


#endif //UMDWITHCLASS3_SETTINGSTAB_H
