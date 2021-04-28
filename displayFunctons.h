//
// Created by theo on 4/24/2021.
//

#ifndef UMDWITHCLASS3_DISPLAYFUNCTONS_H
#define UMDWITHCLASS3_DISPLAYFUNCTONS_H

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_I2C_ADDRESS 0x3C

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class DisplayFunctons {
public:
    DisplayFunctons(){
        init();
    }

    ~DisplayFunctons(){
        delete display;
    }

    void displayWhenNotReading();

    void showReading(bool reading);

    void showBattery();

    void showSensors();

    void showbtAddress();

    void init();

private:
    Adafruit_SSD1306 * display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


};


#endif //UMDWITHCLASS3_DISPLAYFUNCTONS_H
