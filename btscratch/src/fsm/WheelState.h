//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_WHEELSTATE_H
#define BTSCRATCH_WHEELSTATE_H

#include "WheelFsm.h"

#include "../wheels/csw_wheel.h"
#include "../wheels/mcl_wheel.h"
#include "../wheels/csl_wheel.h"

extern csw_wheel csw_wheel;
extern mcl_wheel mcl_wheel;
extern csl_wheel csl_wheel;
extern WheelBase initial_wheel;
extern WheelBase *wheel;

// Forward declaration to resolve circular dependency/include
class WheelFsm;

class WheelState {
public:
    virtual void enter(WheelFsm *fsm) {}

    virtual void exit(WheelFsm *fsm) {}

    virtual void loop(WheelFsm *fsm) {}

    virtual ~WheelState() = default;

    virtual const char* getName() {
        return "Base state";
    }
};


#endif //BTSCRATCH_WHEELSTATE_H

