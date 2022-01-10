//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_CONNECTBLE_H
#define BTSCRATCH_CONNECTBLE_H

#include "../WheelState.h"
#include <bluefruit.h>
#include "../../logger.h"

class ConnectBle : public WheelState {
public:
    ConnectBle(WheelBase *wheel);

    void loop(WheelFsm *fsm) override;

    void enter(WheelFsm *fsm) override;

    static ConnectBle *create(WheelBase *wheel);

private:
    WheelBase *wheel;
    bool _isConnected = false;
    uint16_t _mtu = 0;

    static ConnectBle instance;

    static void connected(uint16_t handle) {
        logger->println("Connected");

        BLEConnection *conn = Bluefruit.Connection(handle);

        char s[40];

        // request PHY changed to 2MB
        sprintf(s, "Request to change PHY: %d", conn->getPHY());
        logger->println(s);
        conn->requestPHY();

        // request to update data length
        sprintf(s, "Request to change Data Length: %d", conn->getDataLength());
        logger->println(s);
        conn->requestDataLengthUpdate();

        // request mtu exchange
        sprintf(s, "Request to change MTU: %d", conn->getMtu());
        logger->println(s);
        conn->requestMtuExchange(247);

        //Capture these with event handler instead
        //        Bluefruit.setEventCallback()
        delay(2000);
        uint16_t mtu = conn->getMtu();
        instance._mtu = mtu;
        sprintf(s, "PHY: %d", conn->getPHY());
        logger->println(s);
        sprintf(s, "Data Length: %d", (conn->getDataLength()));
        logger->println(s);
        sprintf(s, "MTU: %d", mtu);
        logger->println(s);

        instance._isConnected = true;

    }

    static void disconnected(uint16_t conn_hdl, uint8_t reason) {
        logger->println("Disconnected!");
        instance._isConnected = false;
    }

};


#endif //BTSCRATCH_CONNECTBLE_H
