#ifndef BTSCRATCH_WHEELFSM_H
#define BTSCRATCH_WHEELFSM_H

#include "WheelState.h"

class WheelState;

class WheelFsm
{
public:
    WheelFsm();

    inline WheelState *getCurrentState() const { return currentState; }

    void setState(WheelState* newState);

    void loop();

private:
    WheelState* currentState{};
};


#endif //BTSCRATCH_WHEELFSM_H
