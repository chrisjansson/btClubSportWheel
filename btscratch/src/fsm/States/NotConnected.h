//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_NOTCONNECTED_H
#define BTSCRATCH_NOTCONNECTED_H

#include "../WheelState.h"
#include <Adafruit_NeoPixel.h>

class NotConnected : public WheelState {
public:
    void loop(WheelFsm *fsm) override;
    void enter(WheelFsm *fsm) override;

    static NotConnected *create();
private:
    static NotConnected instance;
};


#endif //BTSCRATCH_NOTCONNECTED_H
