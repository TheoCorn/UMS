//
// Created by theo on 8/7/2021.
//

#ifndef UMDWITHCLASS3_UI_H
#define UMDWITHCLASS3_UI_H

#include "Tab.h"
#include "SensorsTab.h"


namespace ui {

    struct coordinates{
        unsigned int x;
        unsigned int y;

        coordinates(unsigned int x, unsigned int y) : x(x), y(y) {}
    };

    class Tab;

    class SensorsTab;


}

#endif //UMDWITHCLASS3_UI_H
