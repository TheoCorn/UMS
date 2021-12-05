// Copyright 2018 Evandro Luis Copercini
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//tweaked by Theodor Capek big thanks to the original author

#ifndef _BLUETOOTH_SERIAL_H_
#define _BLUETOOTH_SERIAL_H_

#include "sdkconfig.h"
#include "asciiMakros.h"

#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BLUEDROID_ENABLED)


#include "Arduino.h"
#include "Stream.h"
#include <esp_spp_api.h>
#include <functional>
#include "SerialCom.h"

typedef std::function<void(const uint8_t *buffer, size_t size)> BluetoothSerialDataCb;
typedef std::function<void(uint32_t num_val)> ConfirmRequestCb;
typedef std::function<void(boolean success)> AuthCompleteCb;

class BluetoothSerial:  public SerialCom
{
    public:

        BluetoothSerial(void);
        ~BluetoothSerial(void);

        bool actualBegin(String localName=String(), bool isMaster=false);
        bool begin() override;
        std::size_t available() override;
        int peek(void);
        int read(void);
        void read(char * c) override;
        size_t write(uint8_t c) override;
        size_t write(const uint8_t *buffer, size_t size) override;
        size_t write(JsonDocument * doc) override;
        size_t write(error::Error* error) override;
        char* getString4Display() override;
        void flush() override;
        void end(void);
        void onData(BluetoothSerialDataCb cb);
        esp_err_t register_callback(esp_spp_cb_t * callback);

        void startConnectionCheck(int duration) override;
        bool hasConnectedDevice() override;

        
        void onConfirmRequest(ConfirmRequestCb cb);
        void onAuthComplete(AuthCompleteCb cb);
        void confirmReply(boolean confirm);

        void enableSSP();
        bool setPin(const char *pin);
        bool connect(String remoteName);
        bool connect(uint8_t remoteAddress[]);
        bool connect();
        bool connected(int timeout=0);
        bool isReady(bool checkMaster=false, int timeout=0);
        bool disconnect();
        bool unpairDevice(uint8_t remoteAddress[]);

    private:
        String local_name;

};

#endif

#endif
