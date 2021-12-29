//
// Created by chris on 2021-12-29.
//

#ifndef BTSCRATCH_CSL_WHEEL_H
#define BTSCRATCH_CSL_WHEEL_H

#include "wheel.h"
#include "../fanatec.h"

class csl_wheel : public wheel {
    void transferData() override {
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x00);
    }

    void process() override {
        wheel.setId(CSLP1XBOX);

        // Joystick / 1st disp
        csl_out.disp = csw7segToCsl(wheel._csw_out.disp[0]);
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x41);
        wheel.hat(csl_in.buttons & 0x1E, true);
        wheel.button(13, csl_in.buttons & 0x01); // hat button

        // Right Line / 2st disp
        csl_out.disp = csw7segToCsl(wheel._csw_out.disp[1]);
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x02);
        wheel.button(11, csl_in.buttons & 0x01); // wrench
        wheel.button(5, csl_in.buttons & 0x04);  // RT
        wheel.button(14, csl_in.buttons & 0x08); // xbox
        wheel.button(6, csl_in.buttons & 0x10);  // RSB

        // Left cluster / 3st disp
        csl_out.disp = csw7segToCsl(wheel._csw_out.disp[2]);
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x44);
        wheel.button(15, csl_in.buttons & 0x01); // left paddle
        wheel.button(9, csl_in.buttons & 0x02);  // lines
        wheel.button(10, csl_in.buttons & 0x04); // squares
        wheel.button(8, csl_in.buttons & 0x08);  // LSB
        wheel.button(7, csl_in.buttons & 0x10);  // LT

        // Right cluster / RGB Led
        csl_out.disp = cswLedsToCsl(wheel._csw_out.leds);
        transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x08);
        wheel.button(16, csl_in.buttons & 0x01); // right paddle
        wheel.button(1, csl_in.buttons & 0x02);  // B
        wheel.button(2, csl_in.buttons & 0x04);  // A
        wheel.button(3, csl_in.buttons & 0x08);  // Y
        wheel.button(4, csl_in.buttons & 0x10);  // X

        wheel.stick(0, 0);
    }

    csl_in_t csl_in;
    csl_out_t csl_out;
};


#endif //BTSCRATCH_CSL_WHEEL_H
