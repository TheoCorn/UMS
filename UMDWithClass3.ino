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
#include "usbSerial.h"
#include "BluetoothSerial.h"

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
#include "SPIFFS.h"
#include "Error.h"
#include "spiffs.hpp"
#include "sysInfo.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define RE_GPIO_PIN_SEL ((1ULL<< BUTTON_PIN) | (1ULL<< REA) | (1ULL<< REB))

#define JSON_SINGLE_SENSOR_SIZE 256;


void doProcess4JsonObj(JsonPair *p);

void onSensorsElementReceive(JsonVariant *v);

void onReadElementReceive(JsonVariant *v);

void onGetElementReceive(JsonVariant *v);

void onStartReading();

void onStopReading();

void sleep();

void onREAISR();

//void reA();
//void reB();


std::map<uint32_t, Sensor *> *sensors;
std::vector<char> *btBuffer;
bool reading = false;

unsigned int sysInfo::screenAddress;
String sysInfo::sn;
sysInfo::BatteryInfo sysInfo::batteryInfo;
unsigned int sysInfo::batteryPercentage;
String sysInfo::comName;
SerialCom *sysInfo::serialCom;
bool sysInfo::isCharging;


DisplayFunctions *mDisplay;
SensorsIdentifierManager *sensorIdentifier;

size_t readJsonCapacity = DEFAULT_JDOC_CAPACITY;


void setup() {
    SPIFFS.begin(true);

    //todo until battery sensor is added
    sysInfo::batteryPercentage = 10;
    sysInfo::isCharging = false;

//    // todo delete before release debug
    Serial.begin(112500);

    sensors = new std::map<uint32_t, Sensor *>;
    btBuffer = new std::vector<char>;

//    gpio_config_t reIo_conf;
//    reIo_conf.intr_type = GPIO_INTR_POSEDGE;
//    reIo_conf.mode = GPIO_MODE_INPUT;
//    reIo_confpin_bit_mask = RE_GPIO_PIN_SEL;
//    reIo_conf.pull_up_en = 1;



    pinMode(traScreen, OUTPUT);
    digitalWrite(traScreen, HIGH);
    pinMode(batteryReadPin, INPUT);

    pinMode(BUTTON_PIN, INPUT);
    pinMode(REA, INPUT);
    pinMode(REB, INPUT);

    digitalWrite(BUTTON_PIN, true);
    digitalWrite(REA, true);
    digitalWrite(REB, true);

    char *sysInfoStr = (char *) spiffs::readFile(SPIFFS, "/sysInfo.json");
    StaticJsonDocument<256> sysInfoDoc;
    deserializeJson(sysInfoDoc, sysInfoStr);

    sysInfo::screenAddress = sysInfoDoc["screenAddress"].as<unsigned int>();


    //todo delete
    Serial.print("screen i2c address: ");
    delay(30);
    Serial.println(sysInfo::screenAddress);
    delay(50);

    unsigned int defCom = sysInfoDoc["defCom"].as<unsigned int>();
    sysInfo::comName = sysInfoDoc["comName"].as<String>();
    sysInfo::serialCom = getSerialCom4EnumPos(defCom);

    sysInfo::sn = sysInfoDoc["sn"].as<String>();

    JsonObject batObj = sysInfoDoc["battery"].as<JsonObject>();
    sysInfo::batteryInfo.name = batObj["name"].as<String>();
    sysInfo::batteryInfo.capacity = batObj["capacity"];



    mDisplay = new DisplayFunctions(sensors);
    sensorIdentifier = new SensorsIdentifierManager();

//    sysInfo::serialCom->startConnectionCheck(5000);

    Wire.begin();

    attachInterrupt(REA, onREAISR, FALLING);
    attachInterrupt(BUTTON_PIN, )


//    //sets up wake up from sleep
//    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);
//    pinMode(sleepPin, INPUT);
//
//    attachInterrupt(sleepPin, sleep, FALLING);


}

void loop() {

    char sRead;
    for (int i = 0; i < sysInfo::serialCom->available(); ++i) {
        sysInfo::serialCom->read(&sRead);

        switch (sRead) {
            case ETX:{
                std::function<void(JsonPair*)> doProcess(doProcess4JsonObj);
                jp::parseJsonWithCycleThru(btBuffer, doProcess);
            }
                break;
            case STX:
                btBuffer->clear();
                break;
            case ACK:
                break;

            default:
                btBuffer->emplace_back(sRead);
                break;
        }
    }


    if (reading) {
        auto doc = new DynamicJsonDocument(readJsonCapacity);
        JsonArray arr = doc->createNestedArray("Sensors");
        for (auto const &sTuple : *sensors) {
            sTuple.second->readSensor(arr);
        }
        sysInfo::serialCom->write(doc);

        delete doc;

    } else {
        auto conflicts = new std::vector<csa::ConflictingAddressStruct *>();
        ss::checkI2C(conflicts, sensors, sensorIdentifier);
        mDisplay->displayWhenNotReading();

        if (!conflicts->empty()) {
            sysInfo::serialCom->write(csa::conflictsToString(conflicts));
        }
        delete conflicts;
    }


}

/**
  identifies json object by key and does the correct action

  @param p JsonPair of the object
*/
void doProcess4JsonObj(JsonPair *p) {
    JsonVariant v = p->value();

    switch (p->key().c_str()[0]) {

        case 's':
            onSensorsElementReceive(&v);
            break;

        case 'r':
            onReadElementReceive(&v);
            break;

        case 'g':
            onGetElementReceive(&v);
            break;

        default:
            break;
    }


}


void onSensorsElementReceive(JsonVariant *v) {
    JsonArray arr = v->as<JsonArray>();

    for (JsonVariant sConf : arr) {
        try {
            JsonObject obj = sConf.as<JsonObject>();
            unsigned int key = obj["rsid"];
            sensors->at(key)->setJson(obj);
        } catch (...) {

            error::Error* errMsg = new error::Error(FAILED_TO_PARSE_JSON_NAME,
                               SET_SENSOR_CONFIG_JSON_FAILURE_MESSAGE,
                               error::Appearance::SNACK_BAR,
                               error::Importance::REQUIRES_USER_ACTION,
                               error::BackgroundAppActions::RESEND);

            sysInfo::serialCom->write(errMsg);
        }
    }


}


void onReadElementReceive(JsonVariant *v) {
    if (v->is<int>()) {
        int locReading = v->as<int>();
        if (locReading == 1) {
            if (reading) return;
            reading = true;
            onStartReading();
        } else {
            if (!reading) return;
            reading = false;
            onStopReading();
        }
    }

}

void onGetElementReceive(JsonVariant *v) {
    DynamicJsonDocument* doc = new DynamicJsonDocument(sensors->size() * 2048);
    JsonArray arr = doc->createNestedArray("SCof");

    uint8_t key;
    Sensor *value;
    for (auto &mPair : *sensors) {
        std::tie(key, value) = mPair;
        value->getJson(arr);
    }

    size_t success = sysInfo::serialCom->write(doc);

    if(!success){
//        error::Error error()
    }

    delete doc;

}

void onStartReading() {

    detachInterrupt(sleepPin);
    mDisplay->displayWhenReading();
    readJsonCapacity = DEFAULT_JDOC_CAPACITY;  //JSON_SINGLE_SENSOR_SIZE * sensors->size();

    delete sensorIdentifier;
}

void onStopReading() {

    sensorIdentifier = new SensorsIdentifierManager();
}

/**
  prepares and deep sleeps the esp32
*/
void sleep() {
    mDisplay->sleep();
    esp_deep_sleep_start();
}

//todo: implement battery percentage
void readBatteryCharge() {
    sysInfo::batteryPercentage = 10;
}

void IRAM_ATTR onREAISR() {
    mDisplay->onREAInterrupt();
}

void IRAM_ATTR onREBISR() {
    mDisplay->onClick();
}
