//
// Created by theo on 4/24/2021.
//

#include "displayFunctions.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#include "gpioNames.h"


void DisplayFunctions::init() {
  pinMode(showAddressPin, INPUT);
  pinMode(traScreen, OUTPUT);
  digitalWrite(traScreen, HIGH);

  pinMode(batteryReadPin, INPUT);
  //Address 0x3C can not be a sensor
  if (!this->display->begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS)) { // Address 0x3C for 128x32
    for (;;); // Don't proceed, loop forever
  }

  display->display();
  delay(5);
  display->clearDisplay();
}

void DisplayFunctions::displayWhenReading() {
  showReading(true);

  display->display();
}

void DisplayFunctions::displayWhenNotReading() {
  Serial.println("display when not reading");
  
  showReading(false);
  Serial.println("after showReading");
  showSensors();
  Serial.println("after showSensors");
  digitalRead(showAddressPin) ? showbtAddress() : showBattery();

  Serial.println("after show address/battery");

  display->display();
}

void DisplayFunctions::showReading(bool reading) {
  display->setTextSize(1);             // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE);        // Draw white text
  display->setCursor(120, 2);
  reading ? display->println("1") : display->println("0");
}


void DisplayFunctions::showBattery() {

  display->drawRect(0, 2, 10, 10, SSD1306_WHITE);
  display->drawRect(3, 0, 4, 2, SSD1306_WHITE);


  display->setTextSize(0.05);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(15, 0);            // Start at top-left corner

  int batery = getBateryProcentige();
  String bateryPr = String (batery);

  display->println(bateryPr + "%");

  if (batCharge < batery) {
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

    if (batery >= 75) {
      display->fillRect(0, 2, 10, 10, SSD1306_WHITE);
    } else if (batery >= 50) {
      display->fillRect(0, 7, 10, 5, SSD1306_WHITE);
    } else if (batery >= 10) {
      display->fillRect(0, 8, 10, 3, SSD1306_WHITE);
    }
  }
  batCharge = batery;

}

void DisplayFunctions::showSensors() {
  display->setTextSize(1);             // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE);        // Draw white text
  display->setCursor(0, 17);

  for (auto const& x : *sensors) {
    display->println(x.second->name());
  }

}

void DisplayFunctions::showbtAddress() {
  display->setTextSize(1);             // Normal 1:1 pixel scale
  display->setTextColor(SSD1306_WHITE);        // Draw white text
  display->setCursor(0, 0);



}

void DisplayFunctions::sleep() {
  display->clearDisplay();
  digitalWrite(traScreen, LOW);
}
