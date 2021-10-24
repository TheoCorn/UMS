//
// Created by theo on 9/27/2021.
//

#include "jsonCommandsfunctions.h"

void jcf::onSensorsElementReceive(JsonVariant *v, std::map<uint32_t, Sensor *> *sensors) {
    JsonArray arr = v->as<JsonArray>();

    for (JsonVariant sConf: arr) {
        try {
//            JsonObject obj = sConf.as<JsonObject>();
//            unsigned int key = obj["rsid"];
//            sensors->at(key)->setJson(obj);

            JsonObject doc = sConf.as<JsonObject>();
            unsigned int key = doc[JSON_KEYWORD_RSID];
            sensors->at(key)->setJson(doc);
        } catch (...) {

            error::Error *errMsg = new error::Error(ERROR__MSG_FAILED_TO_PARSE_JSON_NAME,
                                                    ERROR_MSG__SET_SENSOR_CONFIG_JSON_FAILURE_MESSAGE,
                                                    error::Appearance::SNACK_BAR,
                                                    error::Importance::IMPORTANT,
                                                    error::BackgroundAppActions::RESEND);

            sysInfo::serialCom->write(errMsg);
        }
    }


}


void jcf::onGetElementReceive(JsonVariant *v, std::map<uint32_t, Sensor *> *sensors) {
    DynamicJsonDocument *doc = new DynamicJsonDocument(sensors->size() * 2048);
    JsonArray arr = doc->createNestedArray(JSON_KEYWORD_SENSOR_CONFIG);

    uint8_t key;
    Sensor *value;
    for (auto &mPair: *sensors) {
        std::tie(key, value) = mPair;
        value->getJson(arr);
    }

    size_t success = sysInfo::serialCom->write(doc);

//    if (success < 0) {
//        auto err = error::Error::Error()
//    }

    delete doc;

}
