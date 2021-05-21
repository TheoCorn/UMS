//json
#include <ArduinoJson.h>
#include "JsonParserFunctions.hpp"
#include "JsonSerializer.h"

//sensor stuff
#include "Sensor.hpp"
#include "findSensors.h"

#include "ConflictingSensors.hpp"
#include "AddressIdentifier.hpp"

//serial protocol out
#include "SerialCom.h"
#include "Uart.h"
#include "BluetoothSerial.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

//serial protocols in (4 sensors)
#include <SPI.h>
#include <Wire.h>

//for display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "displayFunctions.h"

//else
#include <iostream>
#include <map>
#include <vector>
#include "gpioNames.h"
#include "asciiMakros.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif



#define DEFAULT_SERIAL_COM BluetoothSerial



void doProcess4JsonObj(JsonPair* p);
void onSensorsElementReceive(JsonVariant* v);
void onReadElementReceive(JsonVariant* v);
void onGetElementReceive(JsonVariant* v);
void onStartReading();
void onStopReading();
void sleep();


// std::vector<Sensor> sensors;
std::map<uint8_t, Sensor*>* sensors;

SerialCom* serialCom;

std::vector<char>* btBuffer;

bool reading = false;


DisplayFunctions* mDisplay;
SensorsIdentifierManager* sensorIdentifier;


void setup() {
  Serial.begin(115200);
  Serial.println("app started");

  sensors = new std::map<uint8_t, Sensor*>;
  Serial.println("after sensors allocated");
  btBuffer = new std::vector<char>;
  Serial.println("after btBuffer alloc");



  pinMode(showAddressPin, INPUT);
  pinMode(traScreen, OUTPUT);
  digitalWrite(traScreen, HIGH);
  pinMode(batteryReadPin, INPUT);


  
  mDisplay = new DisplayFunctions(sensors);
  Serial.println("after display alloc");
  mDisplay->init();
  Serial.println("after display init");
  sensorIdentifier = new SensorsIdentifierManager();

  Serial.println("after sensorIdentifier alloc");

  serialCom = new Uart();
  Serial.println("before startCoonCheck");

  serialCom->startConnectionCheck(5000);

  Serial.println("afterConnCheck");

  Wire.begin();


  //sets up wake up from sleep
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);
  pinMode(sleepPin, INPUT);

  attachInterrupt(sleepPin, sleep, FALLING);

  Serial.println("end of setup");

}

void loop() {
  Serial.println("LOOP");
  char sRead;
  for (int i = 0; i < serialCom->available(); ++i) {
    serialCom->read(&sRead);
    if (sRead != NULL) {
      switch (sRead) {
        case ETX:
          jp::parseJsonWithCycleThru(btBuffer, &doProcess4JsonObj);
          break;
        case STX:
          break;
        case ACK:
          break;

        default:
          btBuffer->emplace_back(sRead);
          break;
      }

    }
  }

  Serial.println("after for char in rx");

  if (reading) {
    mDisplay->displayWhenReading();
  } else {
//    std::vector<csa::ConflictingAddressStruct> conflicts;
//    ss::checkI2C(&conflicts, sensors, sensorIdentifier);
//    mDisplay->displayWhenNotReading();

  }

  Serial.println("end of LOOP");

}

/*
  identifies json object by key and does the correct action

  @param p JsonPair of the object
*/
void doProcess4JsonObj(JsonPair * p) {
  JsonVariant v = p->value();

  switch (p->key().c_str()[0]) {

    case 's':
      onSensorsElementReceive(&v); break;

    case 'r':
      onReadElementReceive(&v); break;

    case 'g': onGetElementReceive(&v); break;
  }

}


void onSensorsElementReceive(JsonVariant * v) {
  JsonObject obj = v->as<JsonObject>();

  for (JsonPair p : obj) {
    try {
      uint8_t key = atoi(p.key().c_str());
      sensors->at(key)->setJson(v);
    } catch (...) {
      //todo: inform pair device update of sensor has failed (create function exceptionOfKeyBuilder that build json to be send)
    }
  }


}



void onReadElementReceive(JsonVariant * v) {
  if (v->is<int>()) {
    int locReading = v->as<int>();
    if (locReading == 1) {
      if (reading) return;
      reading = true;
      onStartReading();
    } else {
      if (reading) return;
      reading = false;
      onStopReading();
    }
  }

}

void onGetElementReceive(JsonVariant * v) {
  DynamicJsonDocument *doc = new DynamicJsonDocument(sensors->size() * 2048);

  uint8_t key;
  Sensor* value;
  for (auto& mPair : *sensors) {
    std::tie(key, value) = mPair;
    value->getJson(doc);
  }

}

void onStartReading() {
  detachInterrupt(sleepPin);
  //  delete sensorIdentifier;
}

void onStopReading() {
  //    sensorIdentifier = new SensorsIdentifierManager();
}

/*
  prepares and deepsleeps the esp32
**/
void sleep() {
  mDisplay->sleep();
  esp_deep_sleep_start();
}
