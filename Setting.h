#include <utility>

//
// Created by theo on 9/20/2021.
//

#ifndef UMDWITHCLASS3_SETTING_H
#define UMDWITHCLASS3_SETTING_H

#include <vector>
#include <functional>
#include "Arduino.h"


/**
 * defines a general setting meant to be displayed on the display
 */
struct GeneralSetting {
    std::vector<char const *> values;
    unsigned int currentIndex;
    const char* name;

    std::function<void(unsigned int)> onSet;


    /**
     *
     * @param name
     * @param values
     * @param currentIndex
     * @param onSet
     */
    GeneralSetting(char const * name, std::vector<const char *> values,
                   unsigned int currentIndex, std::function<void(unsigned int)> onSet) :
            name(name), values(std::move(values)), currentIndex(currentIndex), onSet(std::move(onSet)) {}


    /// if possible decrements index of the setting
    inline void onUp() { if (currentIndex != 0) currentIndex--; }

    /// if possible increments index of the setting
    inline void onDown() { if (currentIndex + 1 < values.size()) currentIndex++; }

    /// calls onSet method with current index
    void set() const { onSet(currentIndex); }
};


#endif //UMDWITHCLASS3_SETTING_H
