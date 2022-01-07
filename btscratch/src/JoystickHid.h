//
// Created by chris on 2022-01-06.
//

#ifndef BTSCRATCH_JOYSTICKHID_H
#define BTSCRATCH_JOYSTICKHID_H

#include <services/BLEHidGeneric.h>
#include "joystick_report_descriptor.h"
#include "manifest.h"

static uint16_t input_report_len[1] = { JOYSTICK_INPUT_REPORT_SIZE };

class JoystickHid : public BLEHidGeneric {
public:
    JoystickHid() : BLEHidGeneric(1, 1, 0) {

    }

    void init() {
        setReportMap(joystick_report_desc, sizeof joystick_report_desc);

#ifdef USE_PNP_ID
        setReportLen(input_report_len, nullptr, nullptr);
#endif
    }
};


#endif //BTSCRATCH_JOYSTICKHID_H
