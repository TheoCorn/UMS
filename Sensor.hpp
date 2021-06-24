
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

    ///unique identification of the Sensor type
    virtual uint32_t sid() = 0;

    /**
     * unique id of the Sensor object at runtime.
     * If there are multiple same sensors they will have the same sid but different rsid
     * rsid stands for runtime specific id
     *
     * @return rsid
     */
    virtual uint32_t rsid() = 0;

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
     * the method has to create 2 variables in the sensor object called "rsid" which is the i2c address or other runtime address...
     * and "sid" which is the sensor sid This sid has to have its config file in the app
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



protected:

    ///@return nested json object with name "Sensor"
    static JsonObject createSensorObject(JsonDocument *doc);

    ///@return nested json object with name "Features"
    static JsonObject createFeaturesArray(JsonObject& obj);

    ///@return nested json object with name "XSettings"
    static JsonObject createXSettingsArray(JsonObject& obj);

    ///@return nested json object with name "ISettings"
    static JsonObject createISettingsArray(JsonObject& obj);



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
     * calls Sensor::createXSettingsArray and calls Sensor::xSettingBuilder for each element of the vector
     *
     * @see Sensor::createXSettingsArray
     * @see Sensor::xSettingBuilder
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
    static void
    generateTemplatedSensorObject(JsonDocument *doc, const uint32_t &rsid, const uint32_t &sid, const bool &isActive);

    /**
     * creates a templated Sensor json nested object
     * so it will contain the name, sid, single feature that is automatically enabled and single XSetting
     *
     * @param doc
     * @param name sensor name
     * @param sid i2cAddress
     * @param feature name of the data set
     * @param xSetting single XSetting
     */
    static void generateTemplatedSensorObject(JsonDocument *doc, const uint32_t& rsid, const uint32_t& sid, String feature,
                                              XSetting& xSetting);


    /**
     *
     * creates a templated Sensor json nested object
     * so it will contain the name, sid, multiple features and XSettings
     *
     * @param doc
     * @param name name of the sensor
     * @param sid i2c address
     * @param features names of the datasets
     * @param activeFeatures which features will be used when reading the Sensor
     * @param xSettings
     */
    static void generateTemplatedSensorObject(JsonDocument *doc, const uint32_t& rsid, const uint32_t& sid,
                                              std::vector<String>& features, std::vector<bool>& activeFeatures,
                                              std::vector<XSetting>& xSettings
    );


    /**
     *
     * creates a templated Sensor json nested object
     * so it will contain the name, sid, multiple features, XSettings and ISettings
     * @param doc
     * @param name
     * @param sid
     * @param features
     * @param activeFeatures
     * @param xSettings
     * @param iSettings
     */
    static void generateTemplatedSensorObject(JsonDocument *doc, const uint32_t& rsid, const uint32_t& sid,
                                              std::vector<String>& features, std::vector<bool>& activeFeatures,
                                              std::vector<XSetting>& xSettings, std::vector<ISetting>& iSettings
    );


};


#endif
