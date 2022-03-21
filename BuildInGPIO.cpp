

//#include "BuildInGPIO.h"
//
//void BuildInGPIO::setUp() {
//    pinMode(ioPins::v5Pin, INPUT);
//    pinMode(ioPins::v3Pin, INPUT);
//}
//
//String BuildInGPIO::getStringForDisplay() {
//    return name();
//}
//
//String BuildInGPIO::getExtendedStringForDisplay() {
//    return Sensor::templatedExtendedString4Display(activeFeaturesVec, dynamic_cast<Sensor*>(this),
//                                                   BuildInGPIO::features_strings);
//}
//
//void BuildInGPIO::readSensor(JsonArray &jra) {
//    Sensor::templatedRead(jra, activeFeaturesVec, rsid(), dynamic_cast<Sensor*>(this));
//}
//
//void BuildInGPIO::saveConfig() {
//    Sensor::settingsSaver(jsonFilePath, activeFeaturesVec);
//}
//
//void BuildInGPIO::getJson(JsonArray &jArr) {
//    Sensor::generateTemplatedSensorObject(jArr, _rsid, sid(), activeFeaturesVec);
//}
//
//void BuildInGPIO::setJson(JsonObject &sConf) {
//    Sensor::JsonSetter(sConf, activeFeaturesVec);
//}
//
//float BuildInGPIO::readFeature(size_t index) {
//    switch (index) {
//        case 0: return analogRead(ioPins::v5Pin);
//        case 1: return analogRead(ioPins::v3Pin);
//    }
//}
