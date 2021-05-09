#include <ArduinoJson.h>
#include "Sensor.hpp"
#include <Adafruit_BMP280.h>
#include "BluetoothSerial.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include <SPI.h>
#include <Wire.h>
#include <iostream>
#include "ConflictingSensors.hpp"
#include "JsonParserFunctions.hpp"
#include "AddressIdentifier.hpp"
#include "MPU9250.hpp"
#include <map>
#include <vector>
#include "displayFunctions.h"
#include "findSensors.h"

//for display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


#define sleepPin 33
#define showAddressPin 18


void doProcess4JsonObj(JsonPair * p);
void onSensorsElementReceive(JsonVariant * v);
void onReadElementReceive(JsonVariant * v);
void onGetElementReceive(&v);
void onStartReading();
void onStopReading();
void sleep();


BluetoothSerial SerialBT;

// std::vector<Sensor> sensors;
std::map<uint8_t, Sensor*> sensors;

std::vector<char> btBuffer;

uint32_t openingBrackets = 0;
uint32_t closingBrackets = 0;

bool reading = false;


DisplayFunctions * mDisplay;
SensorsIdentifierManager * sensorIdentifier;


void setup() {
  mDisplay = new DisplayFunctions(&sensors);
  mDisplay->init();
  sensorIdentifier = new SensorsIdentifierManager();

  //sets up all protocols
  Serial.begin(115200);
  Wire.begin();
  SerialBT.begin("UMD");

  //sets up wake up from sleep
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);
  pinMode(sleepPin, INPUT);

  attachInterrupt(sleepPin, sleep, FALLING);

}

void loop() {
  char sRead;
  int errCode = SerialBT.read(&sRead);
  if (errCode != -1) {
    btBuffer.emplace_back(sRead);
    if (sRead == '{') openingBrackets++;
    if (sRead == '}') closingBrackets++;
    if (openingBrackets == closingBrackets && openingBrackets != 0) {
      openingBrackets = 0;
      closingBrackets = 0;
      jp::parseJsonWithCycleThru(&btBuffer, &doProcess4JsonObj);
    }
  }
  
  if(reading){
      mDisplay->displayWhenReading();
  }else{
      std::vector<csa::ConflictingAddressStruct> conflicts;
      ss::checkI2C(&conflicts, &sensors, sensorIdentifier);
      mDisplay->displayWhenNotReading();
      
  }
  
}

/*
  identifies json object by key and does the correct action

  @param p JsonPair of the object 
*/
void doProcess4JsonObj(JsonPair * p){
  JsonVariant v = p->value();

  switch(p->key().c_str()[0]){

    case 's':
        onSensorsElementReceive(&v); break;

    case 'r':
        onReadElementReceive(&v); break;

    case 'g': recived(&v); break;
  }  

}


void onSensorsElementReceive(JsonVariant * v){
  JsonObject obj = v->as<JsonObject>();

   for(JsonPair p : obj){
    try{
      uint8_t key = atoi(p.key().c_str());
      sensors.at(key)->setJson(v);
    }catch(...){
      //todo: inform pair device update of sensor has failed (create function exeptionOfKeyBuilder taht build json to be send)
    }
  }
  

}



void onReadElementReceive(JsonVariant * v){
  if(v->is<int>()){
    int locReading = v->as<int>();
    if(locReading == 1){
      if(reading) return;
      reading = true;
      onStartReading();
    }else{
      if(reading) return;
      reading = false;
      onStopReading();
    }
  }

}

void onGetElementReceive(JsonVariant * v){
    DynamicJsonDocument *doc = new DynamicJsonDocument(sensors.size() * 2048);

    for(Sensor* s : sensors){
        s->getJson(doc);
    }
}

void onStartReading(){
  detachInterrupt(sleepPin);
//  delete sensorIdentifier;
}

void onStopReading(){
//    sensorIdentifier = new SensorsIdentifierManager();
}

/*
  prepares and deepsleeps the esp32
**/
void sleep(){
  mDisplay->sleep();
  esp_deep_sleep_start();
}
