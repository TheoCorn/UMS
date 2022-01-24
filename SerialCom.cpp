//
// Created by theo on 5/9/2021.
//

#include "SerialCom.h"
#include <ArduinoJson.h>

//SerialComs
#include "BluetoothSerial.h"
#include "usbSerial.h"
#include "Mqtt_server.h"


SerialCom* getSerialCom4EnumPos(unsigned int enumPos){
    switch (enumPos) {
        case sce::BLUETOOTH_SPP : return dynamic_cast<SerialCom*>(new BluetoothSerial()); break;
        case sce::USB_SERIAL : return dynamic_cast<SerialCom*>(new usbSerial()); break;
        case sce::MQTT : return dynamic_cast<SerialCom*>(new Mqtt_server()); break;
        default: return nullptr; break;
    }
}



