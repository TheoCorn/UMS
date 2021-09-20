//
// Created by theo on 9/14/2021.
//

#ifndef UMDWITHCLASS3_SETTINGSTAB_H
#define UMDWITHCLASS3_SETTINGSTAB_H

#define SETTINGS_ARRAY_SIZE 1

#include "ui.h"
#include "sysInfo.h"
#include "SerialCom.h"

/**
 *
 */
struct Setting {
    std::vector<const char *> values;
    size_t currentIndex;
    const char* name;

    Setting(const char* name, std::vector<const char *> values, size_t currentIndex) :
            name(name), values(values), currentIndex(currentIndex) {}

    /**
     * if possible decrements index of the setting
     *
     * @param setting
     */
    inline void onUp();
    inline void onDown();
};

class SettingsTab : public ui::Tab {


    /**
     * array that holds all system settings
     *
     * @see Setting
     */
    Setting settingsArr[SETTINGS_ARRAY_SIZE];

    size_t settArrIndex{0};

    ///variable that describes whether you are iterating @settingArr or the individual Settings
    bool isConfigSetting{false};

    void printSetting(size_t index, size_t xOffset);


public:

    SettingsTab();

    String name() override { return "SETTINGS"; }

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

    bool onUp() override;

    void onDown() override;

    void onClick() override;

};


#endif //UMDWITHCLASS3_SETTINGSTAB_H
