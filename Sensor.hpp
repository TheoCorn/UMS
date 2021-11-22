
#ifndef Sensor_h
#define Sensor_h

#include <stdio.h>
#include <tuple>
#include <vector>
#include <Wire.h>
#include <string>
#include "SPI.h"
#include <ArduinoJson.h>
#include "sensorEnum.h"
#include "JsonParserFunctions.hpp"
#include "Error.h"
#include "json_key_words.h"
//#include "sysInfo.h"
#include "FS.h"
#include "SPIFFS.h"




class Sensor {
public:
    Sensor() = default;

    virtual ~Sensor() = default;

    ///get the name of the Sensor
    virtual String name() = 0;

    /**
     * unique identification of the Sensor type
     *
     * this is the same as the sensors id defined in sensorsEnum and SensorAddresses.json
     */
    virtual uint32_t sid() = 0;

    /**
     * unique id of the Sensor object at runtime.
     * If there are multiple same sensors they will have the same sid but different rsid
     * rsid stands for runtime specific id
     *
     * @return rsid
     */
    virtual uint32_t rsid() = 0;

//    ///the actual i2c address the sensor is using
//    virtual byte currentAddress() = 0;

    ///default set up for setup from user use setJson()
    virtual void setUp() = 0;

    ///used when getting dataFromSensor to display
    virtual String getStringForDisplay() = 0;

    ///used when getting dataFromSensor to display, so more data can be displayed
    virtual String getExtendedStringForDisplay() = 0;

//    virtual char const * getExtendedStringForDisplay() = 0;

    ///used when reading
    ///write JsonArray to the Json document
    virtual void readSensor(JsonArray &jra) = 0;

    virtual void saveConfig() = 0;


    //todo add ISettings
    /**
     * get the current settings in json format.
     *
     * in most cases the Sensor::generateTemplatedSensorObject methods should be sufficient
     * @see Sensor::generateTemplatedSensorObject
     *
     * this is the actual format of the communication
     *
     * the method has to create a nested object in the JsonArray passed to it which name has to be "Sensor" (use Sensor::createSensorObject(JsonArray &doc))
     * (from now known as the sensor object)
     *
     * the method has to create 2 variables in the sensor object called "rsid" which is the i2c address or in the feature other runtime address @see Sensor::_rsid
     * and "sid" which is the sensor id. This sid has to have its config file in the app
     * @see Sensor::sid
     *
     * the method has to create a nested array in the sensor object called "Features"
     * this is a boolean array which corresponds with the features defined by "sid.json" in the user device.
     * each variable in features represents a dataset for the graph (one feature = one 'line' in the graph)
     *
     *   "Features":[true, false, true, false]
     *
     * the method can create a nested array in the sensor object called "XSettings" short for exclusive settings
     * basically forces the user to chose between multiple options
     * meant to be used for stuff like accuracy / range settings...
     * xSettings is a boolean array
     * similarly to features the values correspond to xSettings defined "sid.json" in the user device
     *
     * "XSettings":[0, 0, 1, 4]
     *
     * @param JsonArray&
     */
    virtual void getJson(JsonArray& jArr) = 0;

    /**
     * set setting of the sensor
     *
     * @param sConf the json to set
     */
    virtual void setJson(JsonObject &sConf) = 0;

    /**
     * reads specific feature and return its value
     * @param index
     * @return float value of the feature
     */
    virtual float readFeature(size_t index) { return 0.0f; };




    ///@return nested json object with name "Sensor"
    static JsonObject createSensorObject(JsonArray &doc);

    ///@return nested json object with name "Features"
    static JsonArray createFeaturesArray(JsonObject& obj);

    ///@return nested json object with name "XSettings"
    static JsonArray createXSettingsArray(JsonObject& obj);

    ///@return nested json object with name "ISettings"
    static JsonArray createISettingsArray(JsonObject& obj);



    /**
     * fills in SID and RSID of sensor to a Templated Sensor Object
     *
     * @param obj
     * @param name
     * @param sid
     */
    static void fillBasicInfo(JsonObject& obj, const uint32_t& rsid, const uint32_t& sid);

    /**
     * generates a Features Json object
     *
     * @param sensorObj
     * @param features
     * @param activeFeatures
     */
    static void generateFeatures(JsonObject &sensorObj, std::vector<bool> &activeFeatures);

    /**
     * generates XSettings Json object
     * calls Sensor::createXSettingsArray and add the vector
     *
     * @see Sensor::createXSettingsArray
     *
     * @param sensorObj
     * @param xSettings
     */
    static void generateXSettings(JsonObject& sensorObj, std::vector<unsigned int> &xSettings);

    /**
     * generates ISettings Json object
     * calls Sensor::createISettingsArray and ads each element of the vector
     *
     * @see Sensor::createISettingsArray
     *
     * @param sensorObj
     * @param iSettings
     */
    static void generateISettings(JsonObject& sensorObj, std::vector<bool> &iSettings);

    /**
     * creates the simplest templated Sensor json nested object
     * so it will contain the name, sid, single feature (name of feature is the name parameter) that is automatically enabled no XSettings
     *
     * @param doc
     * @param name sensor name
     * @param sid i2cAddress
     */
    static void generateTemplatedSensorObject(JsonArray& doc, const uint32_t &rsid, const uint32_t &sid, const bool isActive);

//    /**
//     * creates a templated Sensor json nested object
//     * so it will contain the name, sid, single feature that is automatically enabled and single XSetting
//     *
//     * @param doc
//     * @param name sensor name
//     * @param sid i2cAddress
//     * @param feature name of the data set
//     * @param xSetting single XSetting
//     */
//    static void generateTemplatedSensorObject(JsonDocument *doc, const uint32_t& rsid, const uint32_t& sid, String feature,
//                                              const unsigned int& xSetting);

    static void generateTemplatedSensorObject(JsonArray& doc, const uint32_t &rsid, const uint32_t &sid,
                                              const unsigned int xSetting, const bool isActive);


    /**
     *
     * creates a templated Sensor json nested object
     * so it will contain the name, sid, multiple features and XSettings
     *
     * @param doc
     * @param name name of the sensor
     * @param sid i2c address
     * @param activeFeatures which features will be used when reading the Sensor
     * @param xSettings
     */
    static void generateTemplatedSensorObject(JsonArray &doc, const uint32_t &rsid, const uint32_t &sid,
                                              std::vector<bool> &activeFeatures, std::vector<unsigned int> &xSettings);


    /**
     *
     * creates a templated Sensor json nested object
     * so it will contain the name, sid, multiple features, XSettings and ISettings
     * @param doc
     * @param name
     * @param sid
     * @param features
     * @param activeFeatures
     * @param activeFeatures
     * @param xSettings
     */
    static void generateTemplatedSensorObject(JsonArray &doc, const uint32_t &rsid, const uint32_t &sid,
                                              std::vector<bool> &activeFeatures, std::vector<unsigned int> &xSettings,
                                              std::vector<bool> &iSettings);

    /**
     * sets data from json Object to the templated structures
     *
     * @param sConf
     * @param activeFeaturesVec
     */
    static void JsonSetter(JsonObject &sConf,
                           std::vector<bool>& activeFeaturesVec);

    /**
     * sets data from json Object to the templated structures
     *
     * @param sConf
     * @param activeFeaturesVec
     * @param xSettings
     */
    static void JsonSetter(JsonObject &sConf,
                           std::vector<bool>& activeFeaturesVec,
                           std::vector<unsigned int>& xSettings);

    /**
     * sets data from json Object to the templated structures
     *
     * @param sConf
     * @param activeFeaturesVec
     * @param iSettings
     */
    static void JsonSetter(JsonObject &sConf,
                           std::vector<bool>& activeFeaturesVec,
                           std::vector<bool> &iSettings);

    /**
     * sets data from json Object to the templated structures
     *
     * @param sConf
     * @param activeFeaturesVec
     * @param xSettings
     * @param iSettings
     */
    static void JsonSetter(JsonObject &sConf,
                           std::vector<bool>& activeFeaturesVec,
                           std::vector<unsigned int>& xSettings,
                           std::vector<bool> &iSettings);


    static void savedSettingLoader(char const * filename, std::vector<bool>& activeFeaturesVec);

    static void savedSettingsLoader(char const * filename, std::vector<bool>& activeFeaturesVec,
                                    std::vector<unsigned int>& xSettings);

    static void savedSettingsLoader(char const * filename, std::vector<bool>& activeFeaturesVec,
                                    std::vector<bool>& iSettings);

    static void savedSettingsLoader(char const * filename, std::vector<bool>& activeFeaturesVec,
                                    std::vector<unsigned int>& xSettings, std::vector<bool>& iSettings);


    static void settingsSaver(char const * filename, std::vector<bool>& activeFeaturesVec);

    static void settingsSaver(char const * filename, std::vector<bool>& activeFeaturesVec,
                                    std::vector<unsigned int>& xSettings);

    static void settingsSaver(char const * filename, std::vector<bool>& activeFeaturesVec,
                                    std::vector<bool>& iSettings);

    static void settingsSaver(char const * filename, std::vector<bool>& activeFeaturesVec,
                                    std::vector<unsigned int>& xSettings, std::vector<bool>& iSettings);

    static void templatedRead(JsonArray &jra, std::vector<bool>& activeFeaturesVec,
                              uint32_t rsid, Sensor* sensor);

    static String templatedExtendedString4Display(std::vector<bool>& activeFeaturesVec,
                                                  Sensor* sensor,
                                                  char const** FeaturesString);

    // functions taken from adafruit i2c sensor under MIT Licence (made static)

    static bool read(uint8_t *buffer, size_t len, bool stop = true, TwoWire &_wire);
    static bool write(const uint8_t *buffer, size_t len, bool stop = true, const uint8_t *prefix_buffer = NULL,
               size_t prefix_len = 0,
               TwoWire &_wire);
    static bool write_then_read(const uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len,
                         bool stop = false,
                         TwoWire &_wire);
    static bool _read(uint8_t *buffer, size_t len, bool stop, TwoWire &_wire);

                                                  

private:




    /**
     * helper for Sensor::JsonSetter
     *
     * @param sConf
     * @param activeFeaturesVec
     */
    static void features_JsonSetter(JsonObject &sConf,
                                    std::vector<bool>& activeFeaturesVec);

    /**
     * helper for Sensor::JsonSetter
     *
     * @param sConf
     * @param xSettings
     */
    static void xSettings_JsonSetter(JsonObject &sConf,
                                     std::vector<unsigned int>& xSettings);
    /**
     * helper for Sensor::JsonSetter
     *
     * @param sConf
     * @param iSettings
     */
    static void iSettings_JsonSetter(JsonObject &sConf,
                                     std::vector<bool> &iSettings);

};


#endif
