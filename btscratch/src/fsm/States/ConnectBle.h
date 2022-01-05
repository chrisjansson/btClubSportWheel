//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_CONNECTBLE_H
#define BTSCRATCH_CONNECTBLE_H

#include "../WheelState.h"

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

    static void connected(uint16_t conn) {
        Serial.println("Connected!");
        _isConnected = true;
    }

    static void disconnected(uint16_t conn_hdl, uint8_t reason) {
        Serial.println("Disconnected!");
        _isConnected = false;
    }
};


#endif //BTSCRATCH_CONNECTBLE_H
