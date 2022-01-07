//
// Created by chris on 2022-01-05.
//

#include "WheelFsm.h"

#include "WheelState.h"
#include "States/States.h"

WheelFsm::WheelFsm() {
}

void WheelFsm::setState(WheelState *newState) {
    if (currentState != nullptr) {
        currentState->exit(this);
    }
    currentState = newState;
    if (currentState != nullptr) {
        currentState->enter(this);
    }
}

void WheelFsm::loop() {
    if (currentState != nullptr) {
        currentState->loop(this);
    }
}