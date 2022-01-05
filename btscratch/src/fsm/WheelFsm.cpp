//
// Created by chris on 2022-01-05.
//

#include "WheelFsm.h"

#include "WheelState.h"
#include "States/States.h"

WheelFsm::WheelFsm()
{
    currentState = NotConnected::create();
}

void WheelFsm::setState(WheelState* newState)
{
    currentState->exit(this);
    currentState = newState;
    currentState->enter(this);
}

void WheelFsm::loop() {
    currentState->loop(this);
}