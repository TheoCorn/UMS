//
// Created by theo on 12/4/2021.
//

#ifndef UMDWITHCLASS3_MQTT_SERVER_H
#define UMDWITHCLASS3_MQTT_SERVER_H

#include "SerialCom.h"

#include "ArduinoJson.h"
#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <queue>
#include "sysInfo.h"
#include <string.h>
#include "asciiMakros.h"


class Mqtt_server : public SerialCom{

    static constexpr const char* CONTROL_TOPIC = "/control";
    static constexpr const char* OUTPUT_TOPIC = "/output";
    static constexpr const char* ERROR_TOPIC = "/error";
    static constexpr const char* ROOT_TOPIC_PT1 = "/UMD_";

    static constexpr const char* MQTT_CONFIG_FILE_PATH = "/comData/2.json";

    /// (current) control topic
    char* cct;
    /// (current) output topic
    char* cot;
    /// (current) error topic
    char* cet;

    Adafruit_MQTT_Subscribe* sub_control;
    Adafruit_MQTT_Publish* output_pub;

    WiFiClientSecure client;
    Adafruit_MQTT_Client* mqtt;
    static std::queue<char> buffer;

    void MQTT_connect();
    static void control_topic_callback(char *msg, uint16_t len);

public:

    Mqtt_server();

    bool begin() override;

    std::size_t available() override;

    void read(char *c) override;

    size_t write(uint8_t c) override;

    size_t write(uint8_t *buffer, size_t size) override;

    size_t write(JsonDocument *doc) override;

    size_t write(error::Error *error) override;

    void flush() override;

    void startConnectionCheck(int duration) override;

    bool hasConnectedDevice() override;

    char *getString4Display() override;


};


#endif //UMDWITHCLASS3_MQTT_SERVER_H
