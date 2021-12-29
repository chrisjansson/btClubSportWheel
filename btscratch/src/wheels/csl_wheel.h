//
// Created by chris on 2021-12-29.
//

#ifndef BTSCRATCH_CSL_WHEEL_H
#define BTSCRATCH_CSL_WHEEL_H

#include "WheelBase.h"
#include "../fanatec.h"

class csl_wheel : public WheelBase {
public:
    csl_wheel() {
        _csw_out.header = 0xa5;
        _csw_out.id = 0x00;
    }

    void transferData() override {
    }

    void process() override {
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x00);

        setId(CSLP1XBOX);

        // Joystick / 1st disp
        csl_out.disp = csw7segToCsl(_csw_out.disp[0]);
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x41);
        hat(csl_in.buttons & 0x1E, true);
        button(13, csl_in.buttons & 0x01); // hat button

        // Right Line / 2st disp
        csl_out.disp = csw7segToCsl(_csw_out.disp[1]);
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x02);
        button(11, csl_in.buttons & 0x01); // wrench
        button(5, csl_in.buttons & 0x04);  // RT
        button(14, csl_in.buttons & 0x08); // xbox
        button(6, csl_in.buttons & 0x10);  // RSB

        // Left cluster / 3st disp
        csl_out.disp = csw7segToCsl(_csw_out.disp[2]);
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x44);
        button(15, csl_in.buttons & 0x01); // left paddle
        button(9, csl_in.buttons & 0x02);  // lines
        button(10, csl_in.buttons & 0x04); // squares
        button(8, csl_in.buttons & 0x08);  // LSB
        button(7, csl_in.buttons & 0x10);  // LT

        // Right cluster / RGB Led
        csl_out.disp = cswLedsToCsl(_csw_out.leds);
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x08);
        button(16, csl_in.buttons & 0x01); // right paddle
        button(1, csl_in.buttons & 0x02);  // B
        button(2, csl_in.buttons & 0x04);  // A
        button(3, csl_in.buttons & 0x08);  // Y
        button(4, csl_in.buttons & 0x10);  // X

        stick(0, 0);
    }

    csl_in_t csl_in;
    csl_out_t csl_out;
    csw_out_t _csw_out;
};


#endif //BTSCRATCH_CSL_WHEEL_H
