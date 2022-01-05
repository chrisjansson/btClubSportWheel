//
// Created by chris on 2022-01-05.
//

#include <BLEWrapper.h>
#include "ConnectBle.h"
#include "States.h"

ConnectBle::ConnectBle(WheelBase *wheel) : wheel(wheel) {

}

void ConnectBle::enter(WheelFsm *fsm) {
    BLEWrapper::setConnectCallback(connected);
    BLEWrapper::setDisconnectCallback(disconnected);
    BLEWrapper::setup();
}

void ConnectBle::loop(WheelFsm *fsm) {
    if (!_isConnected) {
        return;
    }

    auto nextState = Running::create(wheel);
    fsm->setState(nextState);
}

ConnectBle *ConnectBle::create(WheelBase *wheel) {
    ConnectBle::instance = ConnectBle(wheel);
    return &ConnectBle::instance;
}

ConnectBle ConnectBle::instance = nullptr;
bool ConnectBle::_isConnected = false;


