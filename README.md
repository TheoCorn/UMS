# UMD

Intro                         
============

Welcome to Universal Measurement Device embedded documentation.
UMDe collects data from (i2c) sensors and sends them via various communication protocols to a master computer.
It uses the Arduino framework for fast development time. It can serve as a IoT device or a better multimeter.

## Understanding UMDe
Due to the way Arduino compiles sketches the code can not be structured into subdirectories.
To get a better understanding of the structure look in [Modules](modules.html)

generally you can think about the code as split into 3 modules:
- sensors
- communication
- UI

### sensors
Every implemented sensor is a class extending the [Sensor](#Sensor) class. In most cases there is no need to think about
the logic in the required methods as there are methods for everything in the [Sensor](#Sensor) class. 
See an already implemented sensor like [INA3221](#INA3221) or the docs for the [Sensor](#Sensor) class.

### communication
Every implemented protocol is a class extending the [SerialCom](#SerialCom) class (the protocols do not actually be serial).
Implementing a new protocol should be a quick process.

## UI
The UI part of the project is a little more complicated. 
The UI consists of a 128 by 64 pixel OLED and a clickable rotary encoder.
The rotary encoder uses hardware interrupts and shares the state via an atomic variable. 
The GUI on the screen is seperated into [Tabs](#ui::Tab). 
Changing the UI "module" to make it more abstract would be quite a big undertaking. 
There are so many ways to do UI and there is no reason to change the ui dynamically like the protocol, 
so it is simpler to rewrite the hole UI. 

# Implementing a sensor
### Step 1 
Crete a new c++ class that extends Sensor publicly. 
We are going to be using the templated way of implementing a sensor i.e. using methods from the [Sensor](#Sensor) class. 
Add necessary variables and methods.
```cpp
#include"Sensor.h"

class SENSOR_NAME : public Sensor{
    static constexpr const char* jsonFilePath = "/sensorData/SID.json";
    uint32_t _rsid;
    std::vector<bool> activeFeaturesVec;
    
    //depending on the sensor you might want to use ISetting, both or nither
    std::vector<unsigned int> xSettings;
    std::vector<bool> iSettings;
    static constexpr char const* featuresString[NUM_OF_FEATURES] = {F1, F2, ...};
        
public:     
    uint32_t rsid() override { return _rsid; }
    uint32_t sid() override { return sensorEnum::sensor_name; }

    String name() override { return "NAME"; };

    void setUp() override;

    String getStringForDisplay() override;

    String getExtendedStringForDisplay() override;

    void readSensor(JsonArray &jra) override;

    void saveConfig() override;

    void getJson(JsonArray &jArr) override;

    void setJson(JsonObject &sConf) override;

    float readFeature(size_t index) override;
    
    }
```    
    
### Step 2
Implement these methods in the source file. Here are the methods you should use. 
Don't forget the methods updating the state don't update the actual physical chip. 
Create a private method witch will upload the configuration and call it in [setup](#Sensor::setup) and [setJson](#Sensor::setJson). 
| method | implementation|
| :----: | :-----------: |
| setup  | [savedSettingsLoader](#Sensor::savedSettingsLoader) |
| getStringForDisplay | usually returns [this->name()](#Sensor::name) |
| getExtendedStringForDisplay | return [templatedExtendedString4Display](#Sensor::templatedExtendedString4Display) note that you need to implement [readFeature](#Sensor::readFeature) |
| readSensor | [templatedRead](#Sensor::templatedRead) note that you need to implement [readFeature](#Sensor::readFeature) |
| saveConfig | [settingsSaver](#Sensor::settingsSaver) |
| getJson | [generateTemplatedSensorObject](#Sensor::generateTemplatedSensorObject) |
| setJson | [JsonSetter](#Sensor::JsonSetter) |
| readFeature | create a switch case with all features in the same order as they are in `activeFeaturesVec` and `featuresString` |

### Step 3
Add sensor a new entry to [sensors enum](#sensorEnum) preferably giv it a SID manually (next number), 
though it is not necessary makes finding the sid for a sensor a bit easier sometimes. 

### Step 4 
Create a new file in ./data/SensorData named YOUR_SID.json and add the default configuration. something like: 
```json
{"Features":[true, false, true, true, false, true, true, false, true],"XSettings":[0,2,4,4]}
```

Add your SID to the json (2d) array in ./data/SensorAddresses.json. There are as many elements as there are i2c address, so 
if your sensor can have adress x, y, z add SID to the x, y, z element of the array.
> Use the UMD CLI tool to add SID to SensorAddress.json.
> `python3 UMDscript.py` type  `-h` for help and read the developer commands

# UMDc
UMDcli is a command line tool to communicate with UMDe via USB.
It uses Python3 and uses dependencies specified in requirements.txt.
To download the requirements run

```shell
$ cd UMDc_PATH/
$ pip3 -r requirements.txt 
$ python3 umdc.py
```
