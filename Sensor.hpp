
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
    static JsonObject createSensorObject(JsonDocument *doc);

    ///@return nested json object with name "Features"
    static JsonObject createFeaturesObject(JsonObject& obj);

    ///@return nested json object with name "XSettings"
    static JsonObject createXSettingsObject(JsonObject& obj);

    ///@return nested json object with name "ISettings"
    static JsonObject createISettingsObject(JsonObject& obj);



protected:


    /// holds all data to create a ISetting
    struct ISetting{
        String& name;
        bool& isActive;

        /**
         *
         * @param mName name of the setting
         * @param misActive is active(on/ off)
         */
        ISetting(String& mName, bool& misActive) : name(mName), isActive(misActive) {}
    };



    ///holds  all data to create a XSetting
    struct XSetting{
        String& name;
        std::vector<String>& options;
        int& active;

        /**
         *
         * @param name name of the Setting
         * @param options options
         * @param active active option
         */
        XSetting(String& name, std::vector<String>& options, int& active) : name(name), options(options), active(active) {}
    };


    /**
     * fills in name and uuid of sensor to a Templated Sensor Object
     *
     * @param obj
     * @param name
     * @param uuid
     */
    static void fillBasicInfo(JsonObject& obj, const String& name, const uint8_t& uuid);

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
     *builds an ISetting Json Object from ISetting struct
     *
     * @param iSetting
     * @param iSettingsObj
     *
     * @see Sensor::ISetting
     */
    static void iSettingBuilder(const ISetting& iSetting, JsonObject& iSettingsObj);

    /**
     * generates a Features Json object
     *
     * @param sensorObj
     * @param features
     * @param activeFeatures
     */
    static void generateFeatures(JsonObject& sensorObj, std::vector<String> &features, std::vector<bool> &activeFeatures);

    /**
     * generates XSettings Json object
     * calls Sensor::createXSettingsObject and calls Sensor::xSettingBuilder for each element of the vector
     *
     * @see Sensor::createXSettingsObject
     * @see Sensor::xSettingBuilder
     *
     * @param sensorObj
     * @param xSettings
     */
    static void generateXSettings(JsonObject& sensorObj, std::vector<XSetting> &xSettings);

    /**
     * generates ISettings Json object
     * calls Sensor::createISettingsObject and calls Sensor::iSettingBuilder for each element of the vector
     *
     * @see Sensor::createISettingsObject
     * @see Sensor::iSettingBuilder
     *
     * @param sensorObj
     * @param iSettings
     */
    static void generateISettings(JsonObject& sensorObj, std::vector<ISetting> &iSettings);

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


    /**
     *
     * creates a templated Sensor json nested object
     * so it will contain the name, uuid, multiple features and XSettings
     *
     * @param doc
     * @param name name of the sensor
     * @param uuid i2c address
     * @param features names of the datasets
     * @param activeFeatures which features will be used when reading the Sensor
     * @param xSettings
     */
    static void generateTemplatedSensorObject(JsonDocument *doc, String& name, uint8_t& uuid,
                                              std::vector<String>& features, std::vector<bool>& activeFeatures,
                                              std::vector<XSetting>& xSettings
    );


    /**
     *
     * creates a templated Sensor json nested object
     * so it will contain the name, uuid, multiple features, XSettings and ISettings
     * @param doc
     * @param name
     * @param uuid
     * @param features
     * @param activeFeatures
     * @param xSettings
     * @param iSettings
     */
    static void generateTemplatedSensorObject(JsonDocument *doc, String& name, uint8_t& uuid,
                                              std::vector<String>& features, std::vector<bool>& activeFeatures,
                                              std::vector<XSetting>& xSettings, std::vector<ISetting>& iSettings
    );


};


#endif
