#include <ArduinoJson.h>
#include "AllAboutSensors/Sensor.hpp"
#include <Adafruit_BMP280.h>
#include "bluetooth/BluetoothSerial.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include <SPI.h>
#include <Wire.h>
#include <stdio.h>
#include <iostream>
#include "AllAboutSensors/ConflictingSensors.hpp"
#include "jsonParsing/JsonParserFunctions.hpp"
#include "ActualSensors/mpu9250/M9axisGiro.hpp"
#include <map>
#include <vector>

//for display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define DEF_SENSOR_ARRY_SIZE 5

#define sleepPin 33
#define traScreen 32

#define batteryReadPin 13

#define showAddressPin 18

#define SIZE_OF_SENSOR_CLASS_ARRAY 1 



BluetoothSerial SerialBT;

// std::vector<Sensor> sensors;
std::map<uint8_t, Sensor*> sensors;

std::vector<char> btBuffer;

uint32_t openingBrackets = 0;
uint32_t closingBrackets = 0;

bool reading = false;
float batCharge = 100.0f;

uint8_t nextUUID = 0;




void setup() {
  pinMode(showAddressPin, INPUT);
  pinMode(traScreen, OUTPUT);
  digitalWrite(traScreen, HIGH);

  pinMode(batteryReadPin, INPUT);


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
    displayWhenReading();
  }else{
      std::vector<csa::ConflictingAddressStruct> conflicts;
      ss::checkI2C(&conflicts);
      displayWhenNotReading();

      
  }
  
}

/*
  identifies json object by key and does the correct action

  @param p JsonPair of the object 
*/
void doProcess4JsonObj(JsonPair * p){
  JsonVariant v = p->value();

  switch(hash(p->key().as<char*>)){

    case hash("sensors"): onSensorsElementRecived(&v); break;

    case hash("read"): onReadElementRecived(&v); break;
  }  

}

/*
  used for compering String

  @param data
**/
constexpr uint32_t hash(const char* data) noexcept{
    uint32_t hash = 5381;

    for(const char *c = data; c < data + strlen(data); ++c)
        hash = ((hash << 5) + hash) + (unsigned char) *c;

    return hash;
}

void onSensorsElementRecived(JsonVariant * v){
  JsonObject obj = v->as<JsonObject>();

   for(JsonPair p : obj){
    try{
      sensors.at(p.key()).setJson(v);
    }catch(){
      //todo: inform pair device update of sensor has failed (create function exeptionOfKeyBuilder taht build json to be send)
    }
  }
  

}



void onReadElementRecived(JsonVariant * v){
  if(v->is<int>()){
    int locReading = v->as<int>();
    if(locReadig == 1){
      if(reading) return;
      reading = true;
      onStartReading();
    }else{
      if(reading) return;
      reading = false;
    }
  }

}

void onStartReading(){
  detachInterrupt(sleepPin);
}





void displayWhenReading(){
  showReading(true);
  
  display.display();
}


int getBateryProcentige(){
  return 10;
}

void showbtAddress(void) {
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);
  const uint8_t* point = esp_bt_dev_get_address();

  for (int i = 0; i < 6; i++) {

    char str[3];

    sprintf(str, "%02X", (int)point[i]);
    display.print(str);

    if (i < 5) {
      display.print(":");
    }

  }
}



/*
  prepares and deepsleeps the esp32
**/
void sleep(){
  display.clearDisplay();
  digitalWrite(traScreen, LOW);
  delay(1);
  esp_deep_sleep_start();
}
