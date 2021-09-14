//
// Created by theo on 4/24/2021.
//

#include "displayFunctions.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "gpioNames.h"
#include "SerialCom.h"

#include "sysInfo.h"

#include "ui.h"


void DisplayFunctions::init() {
    pinMode(SCREEN_EN_PIN, OUTPUT);
    digitalWrite(SCREEN_EN_PIN, HIGH);

    pinMode(batteryReadPin, INPUT);
    //Address 0x3C can not be a sensor for me
    if (!this->display->begin(SSD1306_SWITCHCAPVCC, sysInfo::screenAddress)) { // Address 0x3C for 128x32
        for (;;); // Don't proceed, loop forever
    }

    display->display();
    delay(5);
    display->clearDisplay();

//    ui::Tab::sensorIterator = sensors->begin();
}

void DisplayFunctions::displayWhenReading() {
    display->clearDisplay();
    showReading(true);

    display->display();
}

void DisplayFunctions::displayWhenNotReading() {

    if (wasClicked) {
        if (iteratingTabs) {
            Serial.println("ack click");
            iteratingTabs = false;
        } else {
            (*tabIterator)->onClick();
        }
        wasClicked = false;
    }

    if (reaWasLow) reStatusDispatch();

    display->clearDisplay();
    showReading(false);
    showBattery();

    display->setTextSize(1);
    display->setCursor(50, 0);

    if(iteratingTabs){
        display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    }else{
        display->setTextColor(SSD1306_WHITE);
    }

    display->print((*tabIterator)->name());

    display->setTextColor(SSD1306_WHITE);

    (*tabIterator)->render(display, start, end);

    display->display();

//    display->clearDisplay();
//    showReading(false);
//
//    showSensors();
//    showBattery();
//
//    display->display();
}

void DisplayFunctions::showReading(bool reading) {
    display->setTextSize(1);             // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE);        // Draw white text
    display->setCursor(120, 2);
    reading ? display->println('1') : display->println('0');
}


void DisplayFunctions::showBattery() {

    display->drawRect(0, 2, 10, 10, SSD1306_WHITE);
    display->drawRect(3, 0, 4, 2, SSD1306_WHITE);


    display->setTextSize(0.05);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(15, 0);            // Start at top-left corner


    String batteryPr = String(sysInfo::batteryPercentage);

    display->print(sysInfo::batteryPercentage);
    display->print('%');

    if (sysInfo::isCharging) {
        display->fillRect(0, 2, 10, 10, SSD1306_WHITE);
        display->drawPixel(3, 3, SSD1306_BLACK);
        display->drawPixel(4, 3, SSD1306_BLACK);
        display->drawPixel(4, 4, SSD1306_BLACK);
        display->drawPixel(5, 4, SSD1306_BLACK);
        display->drawPixel(5, 5, SSD1306_BLACK);
        display->drawPixel(6, 5, SSD1306_BLACK);
        display->drawPixel(5, 6, SSD1306_BLACK);
        display->drawPixel(4, 7, SSD1306_BLACK);
        display->drawPixel(3, 7, SSD1306_BLACK);
        display->drawPixel(4, 8, SSD1306_BLACK);
        display->drawPixel(5, 9, SSD1306_BLACK);
        display->drawPixel(6, 8, SSD1306_BLACK);

    } else {

        if (sysInfo::batteryPercentage >= 75) {
            display->fillRect(0, 2, 10, 10, SSD1306_WHITE);
        } else if (sysInfo::batteryPercentage >= 50) {
            display->fillRect(0, 7, 10, 5, SSD1306_WHITE);
        } else if (sysInfo::batteryPercentage >= 10) {
            display->fillRect(0, 8, 10, 3, SSD1306_WHITE);
        }
    }

}

//void DisplayFunctions::showSensors() {
//    display->setTextSize(1);             // Normal 1:1 pixel scale
//    display->setTextColor(SSD1306_WHITE);        // Draw white text
//    display->setCursor(0, 17);
//
//    for (auto const &x : *sensors) {
//        display->println(x.second->getStringForDisplay());
//    }
//
//}

void DisplayFunctions::showComInfo() {
    display->setTextSize(1);             // Normal 1:1 pixel scale
    display->setTextColor(SSD1306_WHITE);        // Draw white text
    display->setCursor(0, 0);
    char *str = sysInfo::serialCom->getString4Display();

    display->println(str);
    delete str;

}

void DisplayFunctions::sleep() {
    display->clearDisplay();
    delay(5);
    digitalWrite(SCREEN_EN_PIN, LOW);
    delay(5);
}

void DisplayFunctions::reStatusDispatch() {
    reaWasLow = false;
    if (rebWasLow){
        if (iteratingTabs) {
            if (tabIterator != tabs.begin()) tabIterator--;
            Serial.println("tab--");
        }else{
            iteratingTabs = (*tabIterator)->onUp();
        }
        rebWasLow = false;
    }else{
        if (iteratingTabs) {
            auto endIt = --(tabs.end());
            if (tabIterator != endIt) tabIterator++;
            Serial.println("tab++");
        }else{
            (*tabIterator)->onDown();
        }
    }

}

void DisplayFunctions::onClick() {

}
