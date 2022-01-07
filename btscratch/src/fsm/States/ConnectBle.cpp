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

    if (_mtu - 3 < JOYSTICK_INPUT_REPORT_SIZE) {
        char s[40];
        sprintf(s, "MTU: %d is to low. Requires at least: %d", _mtu, JOYSTICK_INPUT_REPORT_SIZE + 3);
        logger->println(s);
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

