//
// Created by theo on 8/7/2021.
//

#ifndef UMDWITHCLASS3_UI_H
#define UMDWITHCLASS3_UI_H


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <map>
#include "Sensor.hpp"

/**
 * @defgroup UI
 * @{
 */

/**
 * @defgroup top_level
 * @{
 *
 */


namespace ui {

    struct coordinates {
        unsigned int x;
        unsigned int y;

        coordinates(unsigned int x, unsigned int y) : x(x), y(y) {}
    };

    enum class REStatus{
        UP,
        DOWN,
        STATIC
    };

    /**
     * @defgroup Tabs tabs
     * @ingroup UI all files related to UI
     */
    class Tab {

    public:

//        static std::_Rb_tree_iterator<std::pair<const uint32_t, Sensor *>> sensorIterator;

        virtual ~Tab()  = default;

        virtual String name() = 0;

        virtual void render(Adafruit_SSD1306 *display, coordinates &start, coordinates &end) = 0;

        /**
         * @return return true if you want to go back to all tabs
         */
        virtual bool onUp() { return true; }

        virtual void onDown() {}

        virtual void onClick() {}

        virtual void onLongClick() {}

    };

//    class Tab;

//    class SensorsTab;



}

/** @} */

/** @} */


#endif //UMDWITHCLASS3_UI_H
