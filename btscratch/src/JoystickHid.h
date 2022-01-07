//
// Created by chris on 2022-01-06.
//

#ifndef BTSCRATCH_JOYSTICKHID_H
#define BTSCRATCH_JOYSTICKHID_H


#include <services/BLEHidGeneric.h>

class JoystickHid : public BLEHidGeneric {
public:
    JoystickHid() : BLEHidGeneric(1, 1, 0) {

    }
};


#endif //BTSCRATCH_JOYSTICKHID_H
