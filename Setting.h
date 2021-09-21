#include <utility>

//
// Created by theo on 9/20/2021.
//

#ifndef UMDWITHCLASS3_SETTING_H
#define UMDWITHCLASS3_SETTING_H

#include <vector>


/**
 *
 */
struct GeneralSetting {
    std::vector<const char *> values;
    unsigned int currentIndex;
    const char* name;

    Setting(const char* name, std::vector<const char *> values, unsigned int currentIndex) :
            name(name), values(std::move(values)), currentIndex(currentIndex) {}

    /**
     * if possible decrements index of the setting
     *
     * @param setting
     */
    inline void onUp();
    inline void onDown();
};


#endif //UMDWITHCLASS3_SETTING_H
