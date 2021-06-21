
#ifndef Sensor_h
#define Sensor_h

#include <stdio.h>
#include <tuple>
#include <vector>
#include <Wire.h>
#include <string>
#include "SPI.h"
#include <ArduinoJson.h>




class Sensor {
public:
    Sensor() = default;

    virtual ~Sensor() = default;

    ///get the name of the Sensor
    virtual String name() = 0;

    ///the actual i2c address the sensor is using
    virtual byte currentAddress() = 0;

    ///default set up for setup from user use setJson()
    virtual void setUp() = 0;

    ///used when getting dataFromSensor to display
    virtual String getStringForDisplay() = 0;

    ///used when getting dataFromSensor to display, so more data can be displayed
    virtual String getExtendedStringForDisplay() = 0;

    ///used when reading
    ///write JsonObject to the Json document
    virtual void readSensor(JsonDocument *ptrDoc) = 0;

    /**
     * get the current settings in json format
     *
     *
     *
     * the method has to create a nested object in the JsonDocument which name has to be "Sensor" (use Sensor::createSensorObject(JsonDocument*))
     * (from now known as the sensor object)
     * remember that multiple sensors of the same function can be present
     *
     * the method has to create 2 variables in the sensor object called "name" and "uuid" witch is the i2c address
     *
     * the method has to create a nested object in the sensor object called "features"
     * each variable in features represents a dataset for the graph (one feature = one 'line' in graph)
     *
     *   "Features":{
     *    "features": [name0, name1, name2, name3],
     *    "active": [true, false, true, false]
     *   }
     *
     * the method can create a nested object in the sensor object called "XSettings" short for exclusive settings
     * basically forces the user to chose between multiple options
     * meant to be used for stuff like accuracy / range settings...
     *
     * "XSettings":[
     *    "xSetting":{
     *        "name":"setting0",
     *        "options":["option0", "option1", "option2"],
     *        "active":0
     *     },
     *     "xSetting":{
     *        "name":"setting1",
     *        "options":["option0", "option1", "option2"],
     *        "active":2
     *     }
     * ]
     *
     * @param JsonDocument*
     */
    virtual void getJson(JsonDocument *ptrDoc) = 0;

    ///set setting of the sensor
    virtual void setJson(JsonVariant *v) = 0;

    ///@return nested json object with name "Sensor"
    inline JsonObject createSensorObject(JsonDocument *doc);

    ///@return nested json object with name "Features"
    inline JsonObject createFeaturesObject(JsonObject& obj);

    ///@return nested json object with name "XSettings"
    inline JsonObject createXSettingsObject(JsonObject& obj);

    ///@return nested json object with name "ISettings"
    inline JsonObject createISettingsObject(JsonObject& obj);


    /**
     * creates the simplest templated Sensor json nested object
     * so it will contain the name, uuid, single feature (name of feature is the name parameter) that is automatically enabled no XSettings
     *
     * @param doc
     * @param name sensor name
     * @param uuid i2cAddress
     */
    static void generateTemplatedSensorObject(JsonDocument *doc, const String& name, const uint8_t& uuid);

    /**
     * creates the simplest templated Sensor json nested object
     * so it will contain the name, uuid, single feature that is automatically enabled no XSettings
     *
     * @param doc
     * @param name sensor name
     * @param uuid i2cAddress
     * @param feature name of the data set
     */
    static void generateTemplatedSensorObject(JsonDocument *doc, const String& name, const uint8_t& uuid, const String& feature);

    /**
     * creates a templated Sensor json nested object
     * so it will contain the name, uuid, single feature that is automatically enabled and single XSetting
     *
     * @param doc
     * @param name sensor name
     * @param uuid i2cAddress
     * @param feature name of the data set
     * @param xSetting single XSetting
     */
    static void generateTemplatedSensorObject(JsonDocument *doc, String& name, uint8_t uuid, String feature,
                                             XSetting& xSetting);

    static void generateTemplatedSensorObject(JsonDocument *doc, String name, uint8_t uuid,
                                             std::vector<String> features, std::vector<bool> activeFeatures,
                                             std::vector<std::vector<String>> xSettings, std::vector<int> activeXSettings);

    static void generateTemplatedSensorObject(JsonDocument *doc, String& name, uint8_t& uuid,
                                              std::vector<String>& features, std::vector<bool>& activeFeatures,
                                              std::vector<XSetting>& xSettings
                                              );

    static void generateTemplatedSensorObject(JsonDocument *doc, String& name, uint8_t& uuid,
                                              std::vector<String>& features, std::vector<bool>& activeFeatures,
                                              std::vector<XSetting>& xSettings, std::vector<ISetting>& iSettings
    );


protected:
    /**
     * fills in name and uuid of sensor to a Templated Sensor Object
     *
     * @param obj
     * @param name
     * @param uuid
     */
    static void fillBasicInfo(JsonObject& obj, String& name, uint8_t& uuid);

    /**
     * build a xSetting object from XSetting struct and adds it to the XSettings json object
     *
     * @param xSetting
     * @param xSettingsObj
     *
     * @see Sensor::XSetting
     */
    static void xSettingBuilder(const XSetting& xSetting, JsonObject& xSettingsObj);

    /**
     *
     *
     * @param iSetting
     * @param iSettingsObj
     *
     * @see Sensor::ISetting
     */
    static void iSettingBuilder(const ISetting& iSetting, JsonObject& iSettingsObj);

    /**
     * holds all data to create a ISetting
     */
    struct ISetting{
        String& name;
        bool& isActive;

        ISetting(String& mName, bool& misActive) : name(mName), isActive(misActive) {}
    };


    /**
     * hold all data to create a XSetting
     */
    struct XSetting{
        String& name;
        std::vector<String>& options;
        int& active;

        XSetting(String& name, std::vector<String>& options, int& active) : name(name), options(options), active(active) {}
    };
};


#endif
