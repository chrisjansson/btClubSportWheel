//
// Created by chris on 2021-12-29.
//

#ifndef BTSCRATCH_MCL_WHEEL_H
#define BTSCRATCH_MCL_WHEEL_H

#include "wheel.h"
#include "../fanatec.h"

class mcl_wheel : public wheel {
public:
    mcl_wheel(){
        mcl_out.header = 0xa5;
    }

    void transferData() override {
        // McLaren GT3
        transferMclData(&mcl_out, &mcl_in, sizeof(mcl_out.raw));
    }

    void process() override {
        // Wheel ID
        setId(mcl_in.id);

        hat(mcl_in.buttons[0] & 0x0f, false);

        // All buttons
        button(1, mcl_in.buttons[0] & 0x80); // Y
        button(2, mcl_in.buttons[0] & 0x40); // B
        button(3, mcl_in.buttons[0] & 0x20); // X
        button(4, mcl_in.buttons[0] & 0x10); // A
        button(5, mcl_in.buttons[1] & 0x80); // P
        button(6, mcl_in.buttons[1] & 0x40); // N
        button(7, mcl_in.buttons[1] & 0x20); // LSB

        button(9, mcl_in.buttons[1] & 0x04); // RSB

        button(8, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x10));  // switch left up
        button(19, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x10)); // switch left down
        button(10, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x10)); // switch right up
        button(20, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x10)); // switch right down

        button(33, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x20)); // switch left up
        button(34, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x20)); // switch left down
        button(35, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x20)); // switch right up
        button(36, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x20)); // switch right down

        button(37, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x30)); // switch left up
        button(38, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x30)); // switch left down
        button(39, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x30)); // switch right up
        button(40, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x30)); // switch right down

        button(41, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x40)); // switch left up
        button(42, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x40)); // switch left down
        button(43, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x40)); // switch right up
        button(44, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x40)); // switch right down

        button(45, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x50)); // switch left up
        button(46, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x50)); // switch left down
        button(47, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x50)); // switch right up
        button(48, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x50)); // switch right down

        button(49, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x60)); // switch left up
        button(50, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x60)); // switch left down
        button(51, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x60)); // switch right up
        button(52, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x60)); // switch right down

        button(53, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x70)); // switch left up
        button(54, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x70)); // switch left down
        button(55, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x70)); // switch right up
        button(56, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x70)); // switch right down

        button(57, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x80)); // switch left up
        button(58, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x80)); // switch left down
        button(59, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x80)); // switch right up
        button(60, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x80)); // switch right down

        button(61, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x90)); // switch left up
        button(62, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x90)); // switch left down
        button(63, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x90)); // switch right up
        button(64, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x90)); // switch right down

        button(65, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0xA0)); // switch left up
        button(66, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0xA0)); // switch left down
        button(67, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0xA0)); // switch right up
        button(68, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0xA0)); // switch right down

        button(69, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0xB0)); // switch left up
        button(70, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0xB0)); // switch left down
        button(71, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0xB0)); // switch right up
        button(72, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0xB0)); // switch right down

        button(73, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0xC0)); // switch left up
        button(74, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0xC0)); // switch left down
        button(75, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0xC0)); // switch right up
        button(76, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0xC0)); // switch right down

        button(12, mcl_in.buttons[2] & 0x04); // hat button
        button(14, mcl_in.buttons[2] & 0x02); // xbox

        // paddles shitfer
        button(15, mcl_in.buttons[1] & 0x08); // left
        button(16, mcl_in.buttons[1] & 0x01); // right

        int8_t rotary_value = 0;
        rotary_value = mcl_in.encoder;

        if ((mcl_in.garbage[2] & 0x0F) == 0x02 && !mcl_in.axisX) // left clutch fully pressed
        {
            clutch_max = constrain(clutch_max + rotary_value, 0, 0xFF);
        } else {
            button(17, rotary_value <= -1); // left
            button(18, rotary_value >= 1);  // right
        }

        // clutch paddle
        switch (mcl_in.garbage[2] & 0x0F) {
            case 0x01:
                // bite point
                doubleClutch(~mcl_in.axisX, ~mcl_in.axisY);
                break;
            case 0x02:
                // bite point advanced
                doubleClutch(map(~mcl_in.axisX & 0xFF, 0, 0xFF, 0, clutch_max), ~mcl_in.axisY & 0xff);
                break;
            default:
                doubleAxis(~mcl_in.axisX, ~mcl_in.axisY);
        }

        //button(11, mcl_in.buttons[2] & 0x20); // display button

        button(21, ((mcl_in.garbage[3] & 0x0f) == 0x01) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(22, ((mcl_in.garbage[3] & 0x0f) == 0x02) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(23, ((mcl_in.garbage[3] & 0x0f) == 0x03) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(24, ((mcl_in.garbage[3] & 0x0f) == 0x04) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(25, ((mcl_in.garbage[3] & 0x0f) == 0x05) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(26, ((mcl_in.garbage[3] & 0x0f) == 0x06) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(27, ((mcl_in.garbage[3] & 0x0f) == 0x07) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(28, ((mcl_in.garbage[3] & 0x0f) == 0x08) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(29, ((mcl_in.garbage[3] & 0x0f) == 0x09) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(30, ((mcl_in.garbage[3] & 0x0f) == 0x0A) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(31, ((mcl_in.garbage[3] & 0x0f) == 0x0B) && !((mcl_in.buttons[2] & 0x20) == 0x20));
        button(32, ((mcl_in.garbage[3] & 0x0f) == 0x0C) && !((mcl_in.buttons[2] & 0x20) == 0x20));

        stick(0, 0);

#ifdef HAS_DEBUG
        Serial.print("MCL_IN:");
    for (int i = 0; i < sizeof(mcl_in.raw); i++) {

        Serial.print(mcl_in.raw[i], HEX);
        Serial.print(":");
    }
    Serial.println();
#endif
    }

    void clearBase() override {
        clutch_max = 0xFF;
    }

    void clear_display() override {
        mcl_out.raw[1] = 0x11;
        mcl_out.raw[2] = 0x0A;
        mcl_out.raw[3] = 0x0A;
        mcl_out.raw[4] = 0x0A;
        mcl_out.raw[9] = 0x00;
    }

    void display_firmware_version() override {
        String fw_vers = String(mcl_in.fwvers);
        mcl_out.raw[1] = 0x11;
        // Erase all
        mcl_out.raw[2] = 0x0A;
        mcl_out.raw[3] = 0x0A;
        mcl_out.raw[4] = 0x0A;
        //
        mcl_out.raw[2] = fw_vers.charAt(0);
        if (fw_vers.length() > 1)
            mcl_out.raw[3] = fw_vers.charAt(1);
        if (fw_vers.length() > 2)
            mcl_out.raw[4] = fw_vers.charAt(2);
    }

    mcl_in_t mcl_in;
    mcl_out_t mcl_out;
    uint8_t clutch_max = 0xFF;
};


#endif //BTSCRATCH_MCL_WHEEL_H
