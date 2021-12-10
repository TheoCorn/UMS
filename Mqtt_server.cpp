//
// Created by theo on 12/4/2021.
//

#include "Mqtt_server.h"
#include "spiffs.hpp"

std::queue<char> Mqtt_server::buffer;

Mqtt_server::Mqtt_server() {
    cct = new char[39];
    cot = new char[38];
    cet = new char[37];

    strcat(cct, ROOT_TOPIC_PT1);
    strcat(cct, sysInfo::sn.c_str());

    // add root topics to all topics
    strcat(cot, cct);
    strcat(cet, cct);

    //add second order topics to the respected topic
    strcat(cct, CONTROL_TOPIC);
    strcat(cot, OUTPUT_TOPIC);
    strcat(cet, ERROR_TOPIC);


}

bool Mqtt_server::begin() {
    char *cArrJson = (char *) spiffs::readFile(SPIFFS, MQTT_CONFIG_FILE_PATH);
    JsonDocument *doc = jp::parseJson(cArrJson);

    if (doc != nullptr) {

        const char* ssid = (*doc)["ssid"];
        const char* password = (*doc)["psw"];
        const char* key = (*doc)["key"];
        const char* server = (*doc)["server"];
        uint32_t port = (*doc)["port"];
        const char* username = (*doc)["un"];


        WiFi.begin(ssid, password);

        while(WiFi.status() != WL_CONNECTED) {
            delay(300);
            Serial.println("not connected");
        }


        mqtt = new Adafruit_MQTT_Client(&client, server, port, username, key);

        delay(2000);
        MQTT_connect();

    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

//    delete doc;
    delete[] cArrJson;

    sub_control = new Adafruit_MQTT_Subscribe(mqtt, cot);
    sub_control->setCallback(control_topic_callback);
    mqtt->subscribe(sub_control);

    output_pub = new Adafruit_MQTT_Publish(mqtt, cot);
}

std::size_t Mqtt_server::available() {
//    MQTT_connect();
    mqtt->processPackets(2000);
    mqtt->ping();
    return buffer.size();
}

void Mqtt_server::read(char *c) {
    *c = buffer.front();
    buffer.pop();
}

size_t Mqtt_server::write(uint8_t c) {
    output_pub->publish(c);
    return 0;
}

size_t Mqtt_server::write(const uint8_t *buffer, size_t size) {
    output_pub->publish(const_cast<uint8_t*>(buffer), size);
    return 0;
}

size_t Mqtt_server::write(JsonDocument *doc) {
    js::serializeRet * sr = js::serializeDoc(doc);

    output_pub->publish((uint8_t *) sr->buff, sr->bufLen);
    delete sr;
    return 0;
}

size_t Mqtt_server::write(error::Error *error) {
    js::serializeRet * sr = js::serializeError(error);
    output_pub->publish((uint8_t *) sr->buff, sr->bufLen);
    delete sr;
    return 0;
}

void Mqtt_server::flush() {

}

void Mqtt_server::startConnectionCheck(int duration) {

}

bool Mqtt_server::hasConnectedDevice() {
    return WiFi.status() == WL_CONNECTED;
}

char *Mqtt_server::getString4Display() {
    return nullptr;
}

void Mqtt_server::MQTT_connect() {
    int8_t ret;

    // Stop if already connected.
    if (mqtt->connected()) {
        return;
    }

    uint8_t retries = 3;
    while ((ret = mqtt->connect()) != 0) { // connect will return 0 for connected
        mqtt->disconnect();
        delay(5000);  // wait 5 seconds
        retries--;
        if (retries == 0) {
            // basically die and wait for WDT to reset me
            while (1);
        }
    }

}

void Mqtt_server::control_topic_callback(char *msg, uint16_t len) {
    Mqtt_server::buffer.push(av::stx);
    for (uint16_t pos = 0; pos < len; len++){
        Mqtt_server::buffer.push(*(msg + pos));
    }
    Mqtt_server::buffer.push(av::etx);
}
