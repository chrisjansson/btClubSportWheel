//
// Created by chris on 2022-01-05.
//

#ifndef BTSCRATCH_DISPLAYFIRMWARE_H
#define BTSCRATCH_DISPLAYFIRMWARE_H

#include "../WheelState.h"

class DisplayFirmware : public WheelState {
public:
    DisplayFirmware(WheelBase* wheel);

    void loop(WheelFsm *fsm) override;
    void enter(WheelFsm *fsm) override;
    void exit(WheelFsm *fsm) override;
    static DisplayFirmware *create(WheelBase* wheel);
private:
    WheelBase* wheel;
    static DisplayFirmware instance;

    uint32_t _startTime{};
};


#endif //BTSCRATCH_DISPLAYFIRMWARE_H
