//
// Created by chris on 2022-01-05.
//

#include <BLEWrapper.h>
 #include <JoystickHid.h>
#include "ConnectBle.h"
#include "States.h"

extern JoystickHid hidService;

ConnectBle::ConnectBle(WheelBase *wheel) : wheel(wheel) {

}

void ConnectBle::enter(WheelFsm *fsm) {
    BLEWrapper::setConnectCallback(connected);
    BLEWrapper::setDisconnectCallback(disconnected);

    hidService.setOutputReportCallback(0, ConnectBle::OnOutputReport);

    BLEWrapper::setup();
}

void ConnectBle::loop(WheelFsm *fsm) {
    if (!_isConnected) {
        return;
    }

    if (_mtu - 3 < JOYSTICK_INPUT_REPORT_SIZE) {
        char s[50];
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

const char *ConnectBle::getName() {
    return "Connect BLE";
}

