#include <utility>

//
// Created by theo on 9/20/2021.
//

#ifndef UMDWITHCLASS3_SETTING_H
#define UMDWITHCLASS3_SETTING_H

#include <vector>
#include <functional>


/**
 *
 */
struct GeneralSetting {
    std::vector<char const *> values;
    unsigned int currentIndex;
    const char* name;

    std::function<void(unsigned int)>& onSet;

    GeneralSetting(char const * name, std::vector<const char *> values,
                   unsigned int currentIndex, std::function<void(unsigned int)>&& onSet) :
            name(name), values(std::move(values)), currentIndex(currentIndex), onSet(onSet) {}

    /**
     * if possible decrements index of the setting
     *
     * @param setting
     */
    inline void onUp() { if (currentIndex != 0) currentIndex--; }
    inline void onDown() { if (currentIndex + 1 < values.size()) currentIndex++; }

    void set() const {

        Serial.print("index is: ");
        Serial.println(currentIndex);

        onSet(currentIndex);

    }
};


#endif //UMDWITHCLASS3_SETTING_H
