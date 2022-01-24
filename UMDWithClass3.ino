
//different gpios for breadboard version
#define ON_BREADBOARD_WEMOS_LOLIN31

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
#include "jsonTypes.h"
#include "jsonCommandsfunctions.h"
#include "json_key_words.h"
#include "config_json_keywords.h"


//for display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "displayFunctions.h"



#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//battery charge macros
//voltage divider R1 = 330 kOhm, R2 = 680 kOhm
#define MAX_ADC_VOLTAGE 3.3f
#define MAX_ADC_RAW 4095
#define FULL_BATTERY 2.48f   // before voltage divider 3.75f
#define MID_CHARGE 2.4f  //3.7f
#define LOW_CHARGE 2.3f //3.5f
//#define MILI_TO_NORMAL_UNIT 1000



#define RE_GPIO_PIN_SEL ((1ULL<< BUTTON_PIN) | (1ULL<< REA) | (1ULL<< REB))

#define JSON_SINGLE_SENSOR_SIZE 512;


#define GPIO_RE_PIN_SEL  ((1ULL<<BUTTON_PIN) | (1ULL<<REB) | (1ULL<<REA))
#define GPIO_OUTPUT_PIN_SEL ((1ULL<<))

void setDefSysInfo();

void doProcess4JsonObj(JsonPair *p);

void onReadElementReceive(JsonVariant *v);

void onStartReading();

void onStopReading();

void sleep();

void onREAISR();

void onREBISR();

void setBatReader();

//void reA();
//void reB();


std::map<uint32_t, Sensor *> *sensors;
std::vector<char> *comBuffer;
bool reading = false;
double readingPeriod = 0;
double lastReading;

unsigned int sysInfo::screenAddress;
String sysInfo::sn;
sysInfo::BatteryInfo sysInfo::batteryInfo;
unsigned int sysInfo::batteryPercentage;
String sysInfo::comName;
SerialCom *sysInfo::serialCom;
size_t sysInfo::serialComIndex;
bool sysInfo::isCharging;


class BatInfPointers {
public:
    unsigned int* battery_percentage;
    bool* is_charging;

    BatInfPointers(unsigned int* bat_percentage, bool* is_charging) : battery_percentage(bat_percentage),
        is_charging(is_charging) { }
};
TaskHandle_t check_bat_handle = NULL;


DisplayFunctions *mDisplay;
SensorsIdentifierManager *sensorIdentifier;

size_t readJsonCapacity = DEFAULT_JDOC_CAPACITY;

/// the time at start reading
double sTime;

/// conflicts waiting to be resolved
std::vector<csa::ConflictingAddressStruct *> conflicts;


void setup() {
    SPIFFS.begin(true);


    setBatReader();
//    sysInfo::isCharging = false;

        // todo delete before release debug
    Serial.begin(112500);

//    conflicts = new std::vector<csa::ConflictingAddressStruct *>();
    sensors = new std::map<uint32_t, Sensor *>;
    comBuffer = new std::vector<char>;


    //    gpio_config_t io_conf;
    ////disable interrupt
    //    io_conf.intr_type = static_cast<gpio_int_type_t>(GPIO_PIN_INTR_DISABLE);
    ////set as output mode
    //    io_conf.mode = GPIO_MODE_INPUT;
    ////bit mask of the pins that you want to set,e.g.GPIO18/19
    //    io_conf.pin_bit_mask = GPIO_RE_PIN_SEL;
    ////disable pull-down mode
    //    io_conf.pull_down_en = static_cast<gpio_pulldown_t>(0);
    ////enable pull-up mode
    //    io_conf.pull_up_en = static_cast<gpio_pullup_t>(1);
    ////configure GPIO with the given settings
    //    gpio_config(&io_conf);


    pinMode(SCREEN_EN_PIN, OUTPUT);
    digitalWrite(SCREEN_EN_PIN, HIGH);
    pinMode(BATTERY_READ_PIN, INPUT);

    pinMode(BATTERY_IS_CHARGING, INPUT);
    digitalWrite(BATTERY_IS_CHARGING, LOW);

    pinMode(BUTTON_PIN, INPUT);
    pinMode(REA, INPUT);
    pinMode(REB, INPUT);

    digitalWrite(BUTTON_PIN, HIGH);
    digitalWrite(REA, HIGH);
    digitalWrite(REB, HIGH);

    setDefSysInfo();

    Wire.begin();

    sensorIdentifier = new SensorsIdentifierManager();
    auto localConflicts = new std::vector<csa::ConflictingAddressStruct *>();
    ss::checkI2C(localConflicts, sensors, sensorIdentifier);
    delete localConflicts;


    mDisplay = new DisplayFunctions(sensors);

    //    sysInfo::serialCom->startConnectionCheck(5000);


    attachInterrupt(REA, onREAISR, FALLING);
    attachInterrupt(BUTTON_PIN, onREBISR, FALLING);

//    gpio_hold_en(GPIO_NUM_19);
//    gpio_hold_en(GPIO_NUM_34);
//    gpio_hold_en(GPIO_NUM_35);

    gpio_hold_en(GPIO_NUM_0);
    gpio_hold_en(GPIO_NUM_4);
    gpio_hold_en(GPIO_NUM_2);

    //sets up wake up from sleep
//    esp_sleep_enable_ext0_wakeup(GPIO_NUM_19, 1);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 1);
//    pinMode(sleepPin, INPUT);

//    attachInterrupt(sleepPin, sleep, FALLING);


}

void loop() {

    char sRead;
    for (std::size_t i = 0; i < sysInfo::serialCom->available(); ++i) {
        sysInfo::serialCom->read(&sRead);

        switch (sRead) {
            case ETX: {
                std::function<void(JsonPair * )> doProcess(doProcess4JsonObj);
                jp::parseJsonWithCycleThru(comBuffer, doProcess);
            }
                break;
            case STX:
                comBuffer->clear();
                break;
            case ACK:
                break;

            default:
                comBuffer->emplace_back(sRead);
                break;
        }
    }


    if (reading) {
        auto doc = DynamicJsonDocument(readJsonCapacity);
        auto obj = doc.to<JsonObject>();

        double mdelay = readingPeriod - (millis() - lastReading);
        if (mdelay > 0) delay(mdelay);

        lastReading = millis();
        obj[JSON_KEYWORD_TIME] = lastReading - sTime;
        JsonArray arr = obj.createNestedArray(JSON_KEYWORD_SENSORS);
        for (auto const &sTuple: *sensors) {
            sTuple.second->readSensor(arr);
        }
        sysInfo::serialCom->write(&doc);

    } else {
        auto localConflicts = new std::vector<csa::ConflictingAddressStruct *>();
        ss::checkI2C(localConflicts, sensors, sensorIdentifier);
        mDisplay->displayWhenNotReading();

        if (!localConflicts->empty()) {
            sysInfo::serialCom->write(csa::conflictsToJsonDoc(localConflicts));
            conflicts.insert(conflicts.end(), localConflicts->begin(), localConflicts->end());
        }
        delete localConflicts;
    }


}

/**
  identifies json object by key and does the correct action

  @param p JsonPair of the object
*/
void doProcess4JsonObj(JsonPair *p) {
    using namespace jsonTypes;
    using namespace jcf;

    JsonVariant v = p->value();

//    Serial.println(p->key().c_str()[0]);

    switch (p->key().c_str()[0]) {

        case SET_SENSOR_SETTINGS_JSON:
            onSensorsElementReceive(&v, sensors);
            break;

        case SET_READING_JSON:
            onReadElementReceive(&v);
            break;

        case GET_SENSOR_SETTINGS_JSON:
            onGetElementReceive(&v, sensors);
            break;


        case CLEAR_CONFLICT :
            onClearConflict(&v, sensors, conflicts);
            break;


        case CLICK_JSON:
            onREBISR();
            break;
        case UP_JSON :
            mDisplay->reaWasLow = true;
            mDisplay->rebWasLow = true;
            break;
        case DOWN_JSON:
            mDisplay->reaWasLow = true;
            break;

        default: {
            using namespace error;

            auto err = new Error(ERROR_MSG__FAILED_TO_IDENTIFY_COMMAND, "", Appearance::SNACK_BAR,
                                 Importance::MILD, BackgroundAppActions::RESEND);
            sysInfo::serialCom->write(err);
        };
            break;
    }


}


void onReadElementReceive(JsonVariant *v) {
    if (v->is<JsonObject>()) {
        JsonObject obj = v->as<JsonObject>();
        int locReading = obj[READ_JSON_KEYWORD_VALUE];

        if (locReading == 1) {
            if (reading) return;
            reading = true;
            double period = obj[READ_JSON_KEYWORD_PERIOD];
            readingPeriod = period;
            onStartReading();
        } else {
            if (!reading) return;
            reading = false;
            onStopReading();
        }
    }

}


void onStartReading() {

    lastReading = millis();

//    detachInterrupt(sleepPin);
    mDisplay->displayWhenReading();

    //todo chek if working
    readJsonCapacity = DEFAULT_JDOC_CAPACITY;  //JSON_SINGLE_SENSOR_SIZE * sensors->size();

    delete sensorIdentifier;

#ifndef ESP32
#error uses esp32 specific code
#endif
    vTaskDelete(check_bat_handle);

    sTime = millis();
}

void onStopReading() {
    setBatReader();
    sensorIdentifier = new SensorsIdentifierManager();
}

/**
  prepares and deep sleeps the esp32
*/
void sleep() {
    mDisplay->sleep();
    esp_deep_sleep_start();
}

void readBatteryCharge(void* m_bip) {

    auto* bip = (BatInfPointers*)m_bip;
    while(true) {
        float bat_v = analogRead(BATTERY_READ_PIN) * MAX_ADC_VOLTAGE / MAX_ADC_RAW; //* MILI_TO_NORMAL_UNIT;
//        Serial.println(bat_v);
        if (bat_v >= FULL_BATTERY) {
            *(bip->battery_percentage) = 100;
        } else if (bat_v >= MID_CHARGE) {
            *(bip->battery_percentage) = 50;
        } else if (bat_v >= LOW_CHARGE) {
            *(bip->battery_percentage) = 10;
        } else {
            *(bip->battery_percentage) = 0;
        }

        *(bip->is_charging) = digitalRead(BATTERY_IS_CHARGING);

#ifndef ESP32
#error uses esp32 specific code
#endif
        vTaskDelay(60000);
    }

}

#ifndef ESP32
#error uses esp32 specific code
#endif
void setBatReader(){
    auto bip = new BatInfPointers(&sysInfo::batteryPercentage, &sysInfo::isCharging);

    xTaskCreatePinnedToCore(readBatteryCharge, "BatReader", 1024, bip, 2, &check_bat_handle, 0);
}


#ifndef ESP32
#error onREAISR function uses esp32 specific code
#endif
void IRAM_ATTR onREAISR() {
    mDisplay->rebWasLow = !digitalRead(REB);
    mDisplay->reaWasLow = true;
}

#ifndef ESP32
#error onREBISR function uses esp32 specific code
#endif
void IRAM_ATTR onREBISR() {
    mDisplay->wasClicked = true;
}


/**
 * Loads sysInfo from /sysInfo.json
 * if used after setup first free SerialCom
 */
void setDefSysInfo() {
    char *sysInfoStr = (char *) spiffs::readFile(SPIFFS, CONFIG_JSON_FILE_NAME);
    StaticJsonDocument<256> sysInfoDoc;
    deserializeJson(sysInfoDoc, sysInfoStr);

    sysInfo::screenAddress = sysInfoDoc[CONFIG_JSON_SCREEN_ADDRESS].as<unsigned int>();


    sysInfo::serialComIndex = sysInfoDoc[CONFIG_JSON_DEFAULT_COM].as<unsigned int>();

    sysInfo::comName = sysInfoDoc[CONFIG_JSON_COM_NAME].as<String>();
    sysInfo::serialCom = getSerialCom4EnumPos(sysInfo::serialComIndex);

    sysInfo::sn = sysInfoDoc[CONFIG_JSON_SERIAL_NUMBER].as<String>();

    JsonObject batObj = sysInfoDoc[CONFIG_JSON_BATTERY_OBJECT].as<JsonObject>();
    sysInfo::batteryInfo.name = batObj[CONFIG_JSON_BATTERY_NAME].as<String>();
    sysInfo::batteryInfo.capacity = batObj[CONFIG_JSON_BATTERY_CAPACITY];
}
