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

    class Tab {

    public:

        virtual void render(Adafruit_SSD1306* display, coordinates& start, coordinates& end) = 0;

        /**
         * @return return true if you want to go back to all tabs
         */
        virtual bool onUp() { return true; }

        virtual void onDown() {}

        virtual void onClick() {}

        virtual void onLongClick() {}

    };

    class Tab;

    class SensorsTab;


}

#endif //UMDWITHCLASS3_UI_H
