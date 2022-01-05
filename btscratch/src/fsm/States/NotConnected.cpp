//
// Created by chris on 2022-01-05.
//

#include "NotConnected.h"
#include "../../fanatec.h"
#include "States.h"

void NotConnected::loop(WheelFsm *fsm) {
    wheel_type wheelT = detectWheelType();
    if (wheelT == NO_WHEEL) {
        return;
    }

    WheelBase *wheel;
    switch (wheelT) {
        case CSW_WHEEL:
            wheel = &csw_wheel;
            break;
        case CSL_WHEEL:
            wheel = &csl_wheel;
            break;
        case MCL_WHEEL:
            wheel = &mcl_wheel;
            break;
        default:
            wheel = &initial_wheel;
            break;
    }

    wheel->clear();
    Serial.println("Found wheel");
    Serial.println(wheelT);
    fsm->setState(DisplayFirmware::create(wheel));
//    fsm_set_state_DISPLAYING_FIRMWARE();
}

NotConnected NotConnected::instance = NotConnected();

NotConnected *NotConnected::create() {
    return &NotConnected::instance;
}
