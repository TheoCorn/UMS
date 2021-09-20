//
// Created by theo on 9/14/2021.
//

#ifndef UMDWITHCLASS3_SETTINGSTAB_H
#define UMDWITHCLASS3_SETTINGSTAB_H

#define SETTINGS_ARRAY_SIZE 1

#include "ui.h"



class SettingsTab : public ui::Tab{

    struct Setting{
        std::vector<const char *>& values;
        size_t currentIndex;
        String name;

        Setting(String name, std::vector<const char *>&& values, size_t currentIndex) :
            name(name), values(values), currentIndex(currentIndex) {}
    };

    std::vector<Setting> settingsArr[SETTINGS_ARRAY_SIZE];


public:

    SettingsTab();

    String name() override { return "SETTINGS"; }

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

    bool onUp() override;

    void onDown() override;

    void onClick() override;

};


#endif //UMDWITHCLASS3_SETTINGSTAB_H
