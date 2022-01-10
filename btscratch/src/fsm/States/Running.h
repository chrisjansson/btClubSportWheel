//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_RUNNING_H
#define BTSCRATCH_RUNNING_H

#include <BLECharacteristic.h>
#include "../WheelState.h"

class Running : public WheelState {
public:
    Running(WheelBase *wheel);

    void loop(WheelFsm *fsm) override;

    void enter(WheelFsm *fsm) override;

    static Running *create(WheelBase *wheel);

    static void OnOutputReport(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len) {
        memcpy(Running::instance._outputReport, data, min(len, JOYSTICK_OUTPUT_REPORT_SIZE));
        Running::instance._received = len;
    }

private:
    WheelBase *wheel;
    uint8_t _outputReport[JOYSTICK_OUTPUT_REPORT_SIZE];
    uint16_t _received = 0;

    static Running instance;

};

#endif //BTSCRATCH_RUNNING_H
