#include <cstring>
#include "Sensor.hpp"
#include "Wire.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include "Error.h"
#include "spiffs.hpp"



//TwoWire Sensor::wire = Wire;

JsonObject Sensor::createSensorObject(JsonArray &doc) {
    return doc.createNestedObject();
}

JsonArray Sensor::createFeaturesArray(JsonObject &obj) {
    return obj.createNestedArray(JSON_KEYWORD_FEATURES);
}

JsonArray Sensor::createXSettingsArray(JsonObject &obj) {
    return obj.createNestedArray(JSON_KEYWORD_X_SETTINGS);
}

JsonArray Sensor::createISettingsArray(JsonObject &obj) {
    return obj.createNestedArray(JSON_KEYWORD_I_SETTINGS);
}

void Sensor::generateFeatures(JsonObject &sensorObj, std::vector<bool> &activeFeatures) {

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    for(bool element : activeFeatures){
        featuresObj.add(element);
    }



}

void Sensor::generateXSettings(JsonObject& sensorObj, std::vector<unsigned int> &xSettings){

    JsonArray xSettingsObj = createXSettingsArray(sensorObj);
    for (auto const &xs : xSettings) {
        xSettingsObj.add(xs);
    }
}


void Sensor::generateISettings(JsonObject& sensorObj, std::vector<bool> &iSettings){

    JsonArray iSettingsObj = createISettingsArray(sensorObj);
    for(bool const is : iSettings){
        iSettingsObj.add(is);
    }
}


void Sensor::generateTemplatedSensorObject(JsonArray&doc, const uint32_t &rsid, const uint32_t &sid,
                                           const bool isActive) {

    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    featuresObj.add(isActive);

}

void Sensor::generateTemplatedSensorObject(JsonArray&doc, const uint32_t &rsid, const uint32_t &sid,
                                           const unsigned int xSetting, const bool isActive) {

    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    JsonArray featuresObj = createFeaturesArray(sensorObj);
    featuresObj.add(isActive);

    JsonArray xSettingsObj = createXSettingsArray(sensorObj);
    //todo fix nonsense
    unsigned int b = xSetting;
    xSettingsObj.add(b);

}


void Sensor::generateTemplatedSensorObject(JsonArray &doc, const uint32_t &rsid, const uint32_t &sid,
                                           std::vector<bool> &activeFeatures, std::vector<unsigned int> &xSettings) {
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    generateFeatures(sensorObj, activeFeatures);
    generateXSettings(sensorObj, xSettings);

}

void Sensor::generateTemplatedSensorObject(JsonArray &doc, const uint32_t &rsid, const uint32_t &sid,
                                           std::vector<bool> &activeFeatures, std::vector<unsigned int> &xSettings, std::vector<bool> &iSettings) {
    JsonObject sensorObj = createSensorObject(doc);
    fillBasicInfo(sensorObj, rsid, sid);

    generateFeatures(sensorObj, activeFeatures);
    generateXSettings(sensorObj, xSettings);
    generateISettings(sensorObj, iSettings);
}


void Sensor::fillBasicInfo(JsonObject& obj, const uint32_t& rsid, const uint32_t& sid) {
    obj[JSON_KEYWORD_RSID] = rsid;
    obj[JSON_KEYWORD_SID] = sid;
}



void Sensor::templatedRead(JsonArray &jra, std::vector<bool> &activeFeaturesVec,
                           uint32_t rsid, Sensor* sensor) {
    char rsidStr[11];
    itoa(rsid, rsidStr, 10);

    JsonObject rData = jra.createNestedObject();
    rData[JSON_KEYWORD_RSID] = rsidStr;
    JsonArray values = rData.createNestedArray(JSON_KEYWORD_VALUES);

    for (int i = 0; i < activeFeaturesVec.size(); i++) {
        if (activeFeaturesVec[i]) values.add(sensor->readFeature(i));
    }
}

String Sensor::templatedExtendedString4Display(std::vector<bool>& activeFeaturesVec,
                                               Sensor* sensor,
                                               char const** FeaturesString) {

    String s;

    for (int i = 0; i < activeFeaturesVec.size(); i++) {
        float f = sensor->readFeature(i);


        char cBuffer[64];
        int ret = sprintf(&cBuffer[0], "%e", f);
        s += FeaturesString[i];


        s += " ";
        s += activeFeaturesVec[i] ? '1' : '0';
        s += " ";
        s += cBuffer;
        s += "\n";
    }

//    Serial.println(s);

    return s;
}

void Sensor::xSettings_JsonSetter(JsonObject &sConf, std::vector<unsigned int> &xSettings) {
    JsonArray locXSettings = sConf[JSON_KEYWORD_X_SETTINGS];
    xSettings.clear();
    for (JsonVariant v : locXSettings) {
        xSettings.emplace_back(v.as<unsigned int>());
    }
}

void Sensor::iSettings_JsonSetter(JsonObject &sConf, std::vector<bool> &iSettings) {
    JsonArray locISettings = sConf[JSON_KEYWORD_I_SETTINGS];
    iSettings.clear();
    for (JsonVariant v : locISettings) {
        iSettings.emplace_back(v.as<bool>());
    }
}

void Sensor::features_JsonSetter(JsonObject &sConf, std::vector<bool> &activeFeaturesVec) {
    JsonArray features = sConf[JSON_KEYWORD_FEATURES];

    activeFeaturesVec.clear();
    for(JsonVariant v : features){
        activeFeaturesVec.emplace_back(v.as<bool>());
    }
}

void Sensor::JsonSetter(JsonObject &sConf, std::vector<bool> &activeFeaturesVec) {
    features_JsonSetter(sConf, activeFeaturesVec);

}

void Sensor::JsonSetter(JsonObject &sConf,
                        std::vector<bool>& activeFeaturesVec,
                        std::vector<unsigned int>& xSettings) {


    features_JsonSetter(sConf, activeFeaturesVec);
    xSettings_JsonSetter(sConf, xSettings);

}

void Sensor::JsonSetter(JsonObject &sConf, std::vector<bool> &activeFeaturesVec,
                        std::vector<unsigned int> &xSettings,
                        std::vector<bool> &iSettings) {

    features_JsonSetter(sConf, activeFeaturesVec);
    xSettings_JsonSetter(sConf, xSettings);
    iSettings_JsonSetter(sConf, iSettings);

}

void Sensor::JsonSetter(JsonObject &sConf, std::vector<bool> &activeFeaturesVec, std::vector<bool> &iSettings) {
    features_JsonSetter(sConf, activeFeaturesVec);
    iSettings_JsonSetter(sConf, activeFeaturesVec);
}

void Sensor::savedSettingLoader(const char *filename, std::vector<bool> &activeFeaturesVec) {

    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);


    if (doc != nullptr) {
        JsonObject obj = doc->as<JsonObject>();
        JsonSetter(obj, activeFeaturesVec);
    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

    delete[] cArrJson;
}

void Sensor::savedSettingsLoader(const char *filename, std::vector<bool> &activeFeaturesVec,
                                 std::vector<unsigned int> &xSettings) {

    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);


    if (doc != nullptr) {
        JsonObject obj = doc->as<JsonObject>();
        JsonSetter(obj, activeFeaturesVec, xSettings);
        activeFeaturesVec.shrink_to_fit();
        xSettings.shrink_to_fit();
    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

    delete[] cArrJson;

}

void Sensor::savedSettingsLoader(const char *filename, std::vector<bool> &activeFeaturesVec,
                                 std::vector<bool> &iSettings) {

    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);


    if (doc != nullptr) {
        JsonObject obj = doc->as<JsonObject>();
        JsonSetter(obj, activeFeaturesVec, iSettings);
    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

    delete[] cArrJson;
}

void Sensor::savedSettingsLoader(const char *filename, std::vector<bool> &activeFeaturesVec,
                                 std::vector<unsigned int> &xSettings, std::vector<bool> &iSettings) {

    char *cArrJson = (char *) spiffs::readFile(SPIFFS, filename);

    JsonDocument *doc = jp::parseJson(cArrJson);


    if (doc != nullptr) {
        JsonObject obj = doc->as<JsonObject>();
        JsonSetter(obj, activeFeaturesVec, xSettings, iSettings);
    }else{
        throw std::invalid_argument(ERROR_MSG__INVALID_FILE);
    }

    delete[] cArrJson;

}

void Sensor::settingsSaver(const char *filename, std::vector<bool> &activeFeaturesVec,
                           std::vector<unsigned int> &xSettings, std::vector<bool> &iSettings) {

    auto doc = DynamicJsonDocument(DEFAULT_JDOC_CAPACITY);
    JsonObject obj = doc.to<JsonObject>();
    generateFeatures(obj, activeFeaturesVec);
    generateXSettings(obj, xSettings);
    generateISettings(obj, iSettings);

    js::serializeRet* sr = js::serializeDoc(&doc);
    spiffs::writeFile(SPIFFS, filename, sr->buff);

    //todo delete
    Serial.println(sr->buff);

    delete sr;

}

void Sensor::settingsSaver(const char *filename, std::vector<bool> &activeFeaturesVec) {
    auto doc = DynamicJsonDocument(DEFAULT_JDOC_CAPACITY);
    JsonObject obj = doc.to<JsonObject>();
    generateFeatures(obj, activeFeaturesVec);

    js::serializeRet* sr = js::serializeDoc(&doc);
    spiffs::writeFile(SPIFFS, filename, sr->buff);

    //todo delete
    Serial.println(sr->buff);

    delete sr;
}

void Sensor::settingsSaver(const char *filename, std::vector<bool> &activeFeaturesVec,
                           std::vector<unsigned int> &xSettings) {
    auto doc = DynamicJsonDocument(DEFAULT_JDOC_CAPACITY);
    JsonObject obj = doc.to<JsonObject>();
    generateFeatures(obj, activeFeaturesVec);
    generateXSettings(obj, xSettings);

    js::serializeRet* sr = js::serializeDoc(&doc);
    spiffs::writeFile(SPIFFS, filename, sr->buff);

    //todo delete
    Serial.println(sr->buff);

    delete sr;

}

void Sensor::settingsSaver(const char *filename, std::vector<bool> &activeFeaturesVec, std::vector<bool> &iSettings) {
    auto doc = DynamicJsonDocument(DEFAULT_JDOC_CAPACITY);
    JsonObject obj = doc.to<JsonObject>();
    generateFeatures(obj, activeFeaturesVec);
    generateISettings(obj, iSettings);

    js::serializeRet* sr = js::serializeDoc(&doc);
    spiffs::writeFile(SPIFFS, filename, sr->buff);

    //todo delete
    Serial.println(sr->buff);

    delete sr;
}


/*!
 *    @brief  Write a buffer or two to the I2C device. Cannot be more than
 * maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to write. This is const to
 *            ensure the content of this buffer doesn't change.
 *    @param  len Number of bytes from buffer to write
 *    @param  prefix_buffer Pointer to optional array of data to write before
 * buffer. Cannot be more than maxBufferSize() bytes. This is const to
 *            ensure the content of this buffer doesn't change.
 *    @param  prefix_len Number of bytes from prefix buffer to write
 *    @param  stop Whether to send an I2C STOP signal on write
 *    @return True if write was successful, otherwise false.
 */
bool Sensor::write(const uint8_t _addr, const uint8_t *buffer, size_t len, TwoWire &_wire, bool stop, const uint8_t *prefix_buffer, size_t prefix_len) {
    if ((len + prefix_len) > Sensor::max_i2c_buffer_size) {
        // currently not guaranteed to work if more than 32 bytes!
        // we will need to find out if some platforms have larger
        // I2C buffer sizes :/
#ifdef DEBUG_SERIAL
        DEBUG_SERIAL.println(F("\tI2CDevice could not write such a large buffer"));
#endif
        return false;
    }

    _wire.beginTransmission(_addr);

    // Write the prefix data (usually an address)
    if ((prefix_len != 0) && (prefix_buffer != NULL)) {
        if (_wire.write(prefix_buffer, prefix_len) != prefix_len) {
#ifdef DEBUG_SERIAL
            DEBUG_SERIAL.println(F("\tI2CDevice failed to write"));
#endif
            return false;
        }
    }

    // Write the data itself
    if (_wire.write(buffer, len) != len) {
#ifdef DEBUG_SERIAL
        DEBUG_SERIAL.println(F("\tI2CDevice failed to write"));
#endif
        return false;
    }

#ifdef DEBUG_SERIAL

    DEBUG_SERIAL.print(F("\tI2CWRITE @ 0x"));
  DEBUG_SERIAL.print(_addr, HEX);
  DEBUG_SERIAL.print(F(" :: "));
  if ((prefix_len != 0) && (prefix_buffer != NULL)) {
    for (uint16_t i = 0; i < prefix_len; i++) {
      DEBUG_SERIAL.print(F("0x"));
      DEBUG_SERIAL.print(prefix_buffer[i], HEX);
      DEBUG_SERIAL.print(F(", "));
    }
  }
  for (uint16_t i = 0; i < len; i++) {
    DEBUG_SERIAL.print(F("0x"));
    DEBUG_SERIAL.print(buffer[i], HEX);
    DEBUG_SERIAL.print(F(", "));
    if (i % 32 == 31) {
      DEBUG_SERIAL.println();
    }
  }

  if (stop) {
    DEBUG_SERIAL.print("\tSTOP");
  }
#endif

    if (_wire.endTransmission(stop) == 0) {
#ifdef DEBUG_SERIAL
        DEBUG_SERIAL.println();
    // DEBUG_SERIAL.println("Sent!");
#endif
        return true;
    } else {
#ifdef DEBUG_SERIAL
        DEBUG_SERIAL.println("\tFailed to send!");
#endif
        return false;
    }
}

/*!
 *    @brief  Read from I2C into a buffer from the I2C device.
 *    Cannot be more than maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to read into
 *    @param  len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal on read
 *    @return True if read was successful, otherwise false.
 */
bool Sensor::read(const uint8_t _addr, uint8_t *buffer, size_t len, TwoWire &_wire, bool stop) {
    size_t pos = 0;
    while (pos < len) {
        size_t read_len =
                ((len - pos) > Sensor::max_i2c_buffer_size) ? Sensor::max_i2c_buffer_size : (len - pos);
        bool read_stop = (pos < (len - read_len)) ? false : stop;
        if (!Sensor::_read(_addr, buffer + pos, read_len, read_stop))
            return false;
        pos += read_len;
    }
    return true;
}

bool Sensor::_read(const uint8_t _addr, uint8_t *buffer, size_t len, bool stop, TwoWire &_wire) {
#if defined(TinyWireM_h)
    size_t recv = _wire.requestFrom((uint8_t)_addr, (uint8_t)len);
#else
    size_t recv = _wire.requestFrom(_addr, (uint8_t)len, (uint8_t)stop);
#endif

    if (recv != len) {
        // Not enough data available to fulfill our obligation!
#ifdef DEBUG_SERIAL
        DEBUG_SERIAL.print(F("\tI2CDevice did not receive enough data: "));
    DEBUG_SERIAL.println(recv);
#endif
        return false;
    }

    for (uint16_t i = 0; i < len; i++) {
        buffer[i] = _wire.read();
    }

#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.print(F("\tI2CREAD  @ 0x"));
  DEBUG_SERIAL.print(_addr, HEX);
  DEBUG_SERIAL.print(F(" :: "));
  for (uint16_t i = 0; i < len; i++) {
    DEBUG_SERIAL.print(F("0x"));
    DEBUG_SERIAL.print(buffer[i], HEX);
    DEBUG_SERIAL.print(F(", "));
    if (len % 32 == 31) {
      DEBUG_SERIAL.println();
    }
  }
  DEBUG_SERIAL.println();
#endif

    return true;
}

/*!
 *    @brief  Write some data, then read some data from I2C into another buffer.
 *    Cannot be more than maxBufferSize() bytes. The buffers can point to
 *    same/overlapping locations.
 *    @param  write_buffer Pointer to buffer of data to write from
 *    @param  write_len Number of bytes from buffer to write.
 *    @param  read_buffer Pointer to buffer of data to read into.
 *    @param  read_len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal between the write and read
 *    @return True if write & read was successful, otherwise false.
 */
bool
Sensor::write_then_read(const uint8_t _addr, const uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len,
                        TwoWire &_wire, bool stop) {
    if (!Sensor::write(_addr, write_buffer, write_len, _wire,  stop)) {
        return false;
    }

    return Sensor::read(_addr, read_buffer, read_len, _wire);
}
