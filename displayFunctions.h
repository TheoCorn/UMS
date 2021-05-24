//
// Created by theo on 4/24/2021.
//

#ifndef UMDWITHCLASS3_DISPLAYFUNCTIONS_H
#define UMDWITHCLASS3_DISPLAYFUNCTIONS_H

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_I2C_ADDRESS 0x3C

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <map>
#include "Sensor.hpp"


class DisplayFunctions {
public:
    explicit DisplayFunctions(std::map<uint8_t, Sensor*> * sensors){
        this->sensors = sensors;
        this->display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
        init();
    }

    ~DisplayFunctions(){
        delete display;
    }

    void displayWhenReading();

    void displayWhenNotReading();

    void showReading(bool reading);

    void showBattery();

    void showSensors();

    void showComInfo();

    void sleep();


    //todo: implement battery percentage
    static int getBateryProcentige(){
        return 10;
    }

    void init();

private:
    Adafruit_SSD1306 * display;
    float batCharge = 100.0f;
    std::map<uint8_t, Sensor*> * sensors;

};


#endif //UMDWITHCLASS3_DISPLAYFUNCTIONS_H
