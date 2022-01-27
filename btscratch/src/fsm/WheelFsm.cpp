#include "WheelFsm.h"
#include "logger.h"

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