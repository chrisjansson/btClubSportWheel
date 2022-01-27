#ifndef BTSCRATCH_JOYSTICKHID_H
#define BTSCRATCH_JOYSTICKHID_H

#include "BLEHidGeneric2.h"
#include "joystick_report_descriptor.h"
#include "manifest.h"

static uint16_t input_report_len[1] = {JOYSTICK_INPUT_REPORT_SIZE};
static uint16_t output_report_len[1] = {JOYSTICK_OUTPUT_REPORT_SIZE};


class JoystickHid : public BLEHidGeneric2 {
public:
    JoystickHid() : BLEHidGeneric2(1, 1, 0) {

    }

    void init() {
        setReportMap(joystick_report_desc, joystick_report_desc_size);

#ifdef USE_PNP_ID
        setReportLen(input_report_len, output_report_len, nullptr);
#endif
    }
};


#endif //BTSCRATCH_JOYSTICKHID_H
