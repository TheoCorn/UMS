//
// Created by theo on 8/7/2021.
//

#ifndef UMDWITHCLASS3_TAB_H
#define UMDWITHCLASS3_TAB_H


#include <Adafruit_SSD1306.h>


class ui::Tab {

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


#endif //UMDWITHCLASS3_TAB_H
