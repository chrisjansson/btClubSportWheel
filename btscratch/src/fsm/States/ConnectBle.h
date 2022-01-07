//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_CONNECTBLE_H
#define BTSCRATCH_CONNECTBLE_H

#include "../WheelState.h"
#include <bluefruit.h>

class ConnectBle : public WheelState {
public:
    ConnectBle(WheelBase *wheel);

    void loop(WheelFsm *fsm) override;

    void enter(WheelFsm *fsm) override;

    static ConnectBle *create(WheelBase *wheel);

private:
    WheelBase *wheel;
    static ConnectBle instance;

    static bool _isConnected;

    static void connected(uint16_t connh) {
        Serial.println("Connected!");
        _isConnected = true;
        BLEConnection* conn = Bluefruit.Connection(connh);
        Serial.println("Connected");

        // request PHY changed to 2MB
        Serial.println("Request to change PHY");
        Serial.println(conn->getPHY());
        conn->requestPHY();

        // request to update data length
        Serial.println("Request to change Data Length");
        Serial.println(conn->getDataLength());
        conn->requestDataLengthUpdate();

        // request mtu exchange
        Serial.println("Request to change MTU");
        conn->requestMtuExchange(247);

        delay(1000);

        Serial.println(conn->getMtu());
        Serial.println(conn->getDataLength());
        Serial.println(conn->getPHY());

        //TODO: verify mtu and datalength

    }

    static void disconnected(uint16_t conn_hdl, uint8_t reason) {
        Serial.println("Disconnected!");
        _isConnected = false;
    }
};


#endif //BTSCRATCH_CONNECTBLE_H
