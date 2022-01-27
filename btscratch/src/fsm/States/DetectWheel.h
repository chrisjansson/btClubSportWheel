//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_DETECTWHEEL_H
#define BTSCRATCH_DETECTWHEEL_H

#include "../WheelState.h"
#include <Adafruit_NeoPixel.h>

class DetectWheel : public WheelState {
public:
    void loop(WheelFsm *fsm) override;
    void enter(WheelFsm *fsm) override;

    const char * getName() override;

    static DetectWheel *create();
private:
    static DetectWheel instance;
};


#endif //BTSCRATCH_DETECTWHEEL_H
