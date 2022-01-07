//
// Created by chris on 2022-01-05.
//

#include "DisplayFirmware.h"
#include <Arduino.h>
#include "States.h"

void DisplayFirmware::enter(WheelFsm *fsm) {
    _startTime = millis();
}

void DisplayFirmware::loop(WheelFsm *fsm) {
    wheel->display_firmware_version();
    wheel->transferData();
    if (_startTime + 2000 >= millis()) {
        return;
    }

    auto nextState = ConnectBle::create(wheel);
    fsm->setState(nextState);
}

void DisplayFirmware::exit(WheelFsm *fsm) {
    wheel->clear_display();
    wheel->transferData();
}

DisplayFirmware *DisplayFirmware::create(WheelBase *wheel) {
    DisplayFirmware::instance = DisplayFirmware(wheel);
    return &DisplayFirmware::instance;
}

DisplayFirmware::DisplayFirmware(WheelBase *wheel) : wheel(wheel) {

}

DisplayFirmware DisplayFirmware::instance = nullptr;
