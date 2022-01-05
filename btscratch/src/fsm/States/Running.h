//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_RUNNING_H
#define BTSCRATCH_RUNNING_H

#include "../WheelState.h"

class Running : public WheelState {
public:
    Running(WheelBase *wheel);

    void loop(WheelFsm *fsm) override;

    void enter(WheelFsm *fsm) override;

    static Running *create(WheelBase *wheel);

private:
    WheelBase *wheel;
    static Running instance;

};

#endif //BTSCRATCH_RUNNING_H
