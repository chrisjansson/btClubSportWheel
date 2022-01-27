#include "WheelFsm.h"
#include "logger.h"

WheelFsm::WheelFsm() = default;

void WheelFsm::setState(WheelState *newState) {
    if (currentState != nullptr) {
        logger->printf("Exiting state: %s", currentState->getName());
        currentState->exit(this);
    }

    currentState = newState;
    if (currentState != nullptr) {
        logger->printf("Entering state: %s", currentState->getName());
        currentState->enter(this);
    }
}

void WheelFsm::loop() {
    if (currentState != nullptr) {
        currentState->loop(this);
    }
}