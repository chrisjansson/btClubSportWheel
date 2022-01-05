//
// Created by chris on 2021-12-30.
//

#ifndef BTSCRATCH_BLEWRAPPER_H
#define BTSCRATCH_BLEWRAPPER_H

#include <bluefruit_common.h>

class BLEWrapper {
public:
    static void setup();
    static void setConnectCallback(ble_connect_callback_t fp);
    static void setDisconnectCallback(ble_disconnect_callback_t fp);
private:
    static void startAdvertising();
    static void setupServices();

    static void printPnpId();
    static bool usePnpId;
};


#endif //BTSCRATCH_BLEWRAPPER_H
