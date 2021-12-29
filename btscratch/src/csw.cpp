#line 1 "csw.cpp"
/*
 * Copyright (C) 2015 darknao
 * https://github.com/darknao/btClubSportWheel
 *
 * This file is part of btClubSportWheel.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// #include "WProgram.h"
#include "Arduino.h"

#include "fanatec.h"
#include "Debouncer.h"
#include "Joystick.h"
#include "wheel.h"

#define MAX_SPEED 5000

uint8_t hid_data[35];
uint32_t max_delay = 150000; // overhead if below 120ms
// 400 is too short with fanaleds

bool in_changed;

/* Receive HID host to device */
void receive_hid_from_host(uint8_t link_id, uint16_t data_length, const uint8_t *data);

void setup();

void setup2();

void loop();

void idle();

void show_firmware_version();

/* Wheel inputs */
void whClear();

csw_in_t csw_in;

csl_in_t csl_in;
csl_out_t csl_out;

mcl_in_t mcl_in;
mcl_out_t mcl_out;

bool show_fwvers;
uint32_t timing;
uint32_t fwShownForDuration;
uint32_t usb_time;

uint8_t hid_pck[7];

wheel wheel;

byte rotary_debounce = 0;
int8_t rotary_value = 0;
uint8_t main_link_id = 1;
uint8_t left_paddle = 0;
uint8_t right_paddle = 0;
uint8_t clutch_max = 0xFF;

void setup() {
    fsetup();

    whClear();

    // prebuild output packet
    memset(wheel._csw_out.raw, 0, sizeof(csw_out_t));
    memset(csl_out.raw, 0, sizeof(csl_out_t));
    memset(mcl_out.raw, 0, sizeof(mcl_out_t));
    wheel._csw_out.header = 0xa5;
    mcl_out.header = 0xa5;
    wheel._csw_out.id = 0x00;
    setup2();

// debug
#ifdef HAS_DEBUG

    Serial.begin(115200);
    Serial.println("MCU Ready");
    for (int i = 0; i < 100; ++i) {
        delay(100);
        Serial.print(".");
        /* code */
    }

    Serial.println("check for active connection...");
#endif

    timing = micros();
    millis();
    usb_time = micros();
}

void csw_loop() {
    // csw stuff
    // Read Fanatec Packet

    //csw_out.raw[9] = 0x0F; // xbox light
    transferCswData(&wheel._csw_out, &csw_in, sizeof(wheel._csw_out.raw));
    show_firmware_version();

#ifdef HAS_DEBUG
    Serial.print("CSW_IN:");
    for (int i = 0; i < sizeof(csw_in.raw); i++) {

        Serial.print(csw_in.raw[i], HEX);
        Serial.print(":");
    }
    Serial.println();
#endif

    // Wheel ID
    wheel.setId(csw_in.id);

    // Left stick
    wheel.stick(csw_in.axisX, csw_in.axisY);

    // All buttons
    wheel.button(1, csw_in.buttons[0] & 0x80);  // first top right
    wheel.button(2, csw_in.buttons[0] & 0x40);  // middle right
    wheel.button(3, csw_in.buttons[0] & 0x20);  // second top right
    wheel.button(4, csw_in.buttons[0] & 0x10);  // bottom right
    wheel.button(5, csw_in.buttons[1] & 0x80);  // third center
    wheel.button(6, csw_in.buttons[1] & 0x40);  // first center
    wheel.button(7, csw_in.buttons[1] & 0x20);  // middle left
    wheel.button(9, csw_in.buttons[1] & 0x04);  // bottom left
    wheel.button(11, csw_in.buttons[2] & 0x08); // second center
    wheel.button(12, csw_in.buttons[2] & 0x04); // stick button

    // paddles shitfer

    left_paddle = csw_in.buttons[1] & 0x08;
    right_paddle = csw_in.buttons[1] & 0x01;

    if (csw_in.id == XBOXHUB) {
        left_paddle |= csw_in.garbage[0] & 0x02;
        right_paddle |= csw_in.garbage[0] & 0x08;
    }

    wheel.button(15, left_paddle); // left
    wheel.button(16, right_paddle); // right

    rotary_value = csw_in.encoder;

    if (csw_in.id != CSLMCLGT3) {
        wheel.button(8, csw_in.buttons[1] & 0x10);  // first top left
        wheel.button(10, csw_in.buttons[1] & 0x02); // second top left
        wheel.button(13, csw_in.buttons[2] & 0x02); // hat button
        wheel.button(14, csw_in.buttons[2] & 0x20); // display button

        wheel.button(17, rotary_value == -1); // left
        wheel.button(18, rotary_value == 1);  // right
    }

    if (csw_in.id == UNIHUB || csw_in.id == XBOXHUB) {
        // Uni Hub extra buttons
        // BUT_5 array (optional 3 buttons)
        wheel.button(19, csw_in.btnHub[0] & 0x08);
        wheel.button(20, csw_in.btnHub[0] & 0x10);
        wheel.button(21, csw_in.btnHub[0] & 0x20);

        // Playstation buttons
        wheel.button(22, csw_in.btnPS[0] & 0x01);
        wheel.button(23, csw_in.btnPS[0] & 0x02);
        wheel.button(24, csw_in.btnPS[0] & 0x04);
        wheel.button(25, csw_in.btnPS[0] & 0x08);
        wheel.button(26, csw_in.btnPS[0] & 0x10);
        wheel.button(27, csw_in.btnPS[0] & 0x20);
        wheel.button(28, csw_in.btnPS[0] & 0x40);
        wheel.button(29, csw_in.btnPS[0] & 0x80);

        wheel.button(30, csw_in.btnPS[1] & 0x01);
        wheel.button(31, csw_in.btnPS[1] & 0x02);
        wheel.button(32, csw_in.btnPS[1] & 0x04);
        wheel.button(33, csw_in.btnPS[1] & 0x08);
        wheel.button(34, csw_in.btnPS[1] & 0x10);
        wheel.button(35, csw_in.btnPS[1] & 0x20);
        wheel.button(36, csw_in.btnPS[1] & 0x40);
        wheel.button(37, csw_in.btnPS[1] & 0x80);

    }

    if (csw_in.id == XBOXHUB) {
        // Xbox Hub has 1 extra button
        wheel.button(38, csw_in.btnHub[1] & 0x08);

        //APM
        wheel.button(39, csw_in.garbage[0] & 0x01);
        // wheel.button(40, csw_in.garbage[0] & 0x02);
        wheel.button(41, csw_in.garbage[0] & 0x04);
        // wheel.button(42, csw_in.garbage[0] & 0x08);
        wheel.doubleAxis(csw_in.garbage[1], csw_in.garbage[2]);
    }

    wheel.hat(csw_in.buttons[0] & 0x0f, false);

    // Serial.println(String("button: ") + hid_data[3]);

    if (csw_in.id == CSLMCLGT3) {
        wheel.button(8, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x10));  // switch left up
        wheel.button(19,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x10)); // switch left down
        wheel.button(10, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x10)); // switch right up
        wheel.button(20,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x10)); // switch right down

        wheel.button(33, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x20)); // switch left up
        wheel.button(34,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x20)); // switch left down
        wheel.button(35, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x20)); // switch right up
        wheel.button(36,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x20)); // switch right down

        wheel.button(37, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x30)); // switch left up
        wheel.button(38,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x30)); // switch left down
        wheel.button(39, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x30)); // switch right up
        wheel.button(40,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x30)); // switch right down

        wheel.button(41, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x40)); // switch left up
        wheel.button(42,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x40)); // switch left down
        wheel.button(43, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x40)); // switch right up
        wheel.button(44,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x40)); // switch right down

        wheel.button(45, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x50)); // switch left up
        wheel.button(46,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x50)); // switch left down
        wheel.button(47, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x50)); // switch right up
        wheel.button(48,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x50)); // switch right down

        wheel.button(49, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x60)); // switch left up
        wheel.button(50,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x60)); // switch left down
        wheel.button(51, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x60)); // switch right up
        wheel.button(52,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x60)); // switch right down

        wheel.button(53, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x70)); // switch left up
        wheel.button(54,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x70)); // switch left down
        wheel.button(55, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x70)); // switch right up
        wheel.button(56,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x70)); // switch right down

        wheel.button(57, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x80)); // switch left up
        wheel.button(58,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x80)); // switch left down
        wheel.button(59, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x80)); // switch right up
        wheel.button(60,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x80)); // switch right down

        wheel.button(61, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0x90)); // switch left up
        wheel.button(62,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0x90)); // switch left down
        wheel.button(63, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0x90)); // switch right up
        wheel.button(64,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0x90)); // switch right down

        wheel.button(65, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0xA0)); // switch left up
        wheel.button(66,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0xA0)); // switch left down
        wheel.button(67, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0xA0)); // switch right up
        wheel.button(68,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0xA0)); // switch right down

        wheel.button(69, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0xB0)); // switch left up
        wheel.button(70,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0xB0)); // switch left down
        wheel.button(71, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0xB0)); // switch right up
        wheel.button(72,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0xB0)); // switch right down

        wheel.button(73, (csw_in.buttons[1] & 0x10) && ((csw_in.garbage[3] & 0xF0) == 0xC0)); // switch left up
        wheel.button(74,
                     (csw_in.buttons[2] & 0x80) && ((csw_in.garbage[3] & 0xF0) == 0xC0)); // switch left down
        wheel.button(75, (csw_in.buttons[1] & 0x02) && ((csw_in.garbage[3] & 0xF0) == 0xC0)); // switch right up
        wheel.button(76,
                     (csw_in.buttons[2] & 0x40) && ((csw_in.garbage[3] & 0xF0) == 0xC0)); // switch right down

        wheel.button(14, csw_in.buttons[2] & 0x02); // xbox

        if ((csw_in.garbage[2] & 0x0F) == 0x02 && !csw_in.axisX) // left clutch fully pressed
        {
            clutch_max = constrain(clutch_max + rotary_value, 0, 0xFF);
        } else {
            wheel.button(17, rotary_value <= -1); // left
            wheel.button(18, rotary_value >= 1);  // right
        }

        // clutch paddle
        switch (csw_in.garbage[2] & 0x0F) {
            case 0x01:
                // bite point
                wheel.doubleClutch(~csw_in.axisX, ~csw_in.axisY);
                break;
            case 0x02:
                // bite point advanced
                wheel.doubleClutch(map(~csw_in.axisX & 0xFF, 0, 0xFF, 0, clutch_max), ~csw_in.axisY & 0xff);
                break;
            default:
                wheel.doubleAxis(~csw_in.axisX, ~csw_in.axisY);
        }

        //wheel.button(11, mcl_in.buttons[2] & 0x20); // display button

        wheel.button(21, ((csw_in.garbage[3] & 0x0f) == 0x01) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(22, ((csw_in.garbage[3] & 0x0f) == 0x02) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(23, ((csw_in.garbage[3] & 0x0f) == 0x03) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(24, ((csw_in.garbage[3] & 0x0f) == 0x04) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(25, ((csw_in.garbage[3] & 0x0f) == 0x05) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(26, ((csw_in.garbage[3] & 0x0f) == 0x06) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(27, ((csw_in.garbage[3] & 0x0f) == 0x07) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(28, ((csw_in.garbage[3] & 0x0f) == 0x08) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(29, ((csw_in.garbage[3] & 0x0f) == 0x09) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(30, ((csw_in.garbage[3] & 0x0f) == 0x0A) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(31, ((csw_in.garbage[3] & 0x0f) == 0x0B) && !((csw_in.buttons[2] & 0x20) == 0x20));
        wheel.button(32, ((csw_in.garbage[3] & 0x0f) == 0x0C) && !((csw_in.buttons[2] & 0x20) == 0x20));

        wheel.stick(0, 0);
    }
}

void csl_loop() {
    // csl stuff
    transferCslData(&csl_out, &csl_in, sizeof(csl_out.raw), 0x00);
    wheel.setId(CSLP1XBOX);
    show_firmware_version();

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

void mcl_loop() {
    // McLaren GT3

    transferMclData(&mcl_out, &mcl_in, sizeof(mcl_out.raw));
    show_firmware_version();

    // Wheel ID
    wheel.setId(mcl_in.id);

    wheel.hat(mcl_in.buttons[0] & 0x0f, false);

    // All buttons
    wheel.button(1, mcl_in.buttons[0] & 0x80); // Y
    wheel.button(2, mcl_in.buttons[0] & 0x40); // B
    wheel.button(3, mcl_in.buttons[0] & 0x20); // X
    wheel.button(4, mcl_in.buttons[0] & 0x10); // A
    wheel.button(5, mcl_in.buttons[1] & 0x80); // P
    wheel.button(6, mcl_in.buttons[1] & 0x40); // N
    wheel.button(7, mcl_in.buttons[1] & 0x20); // LSB

    wheel.button(9, mcl_in.buttons[1] & 0x04); // RSB

    wheel.button(8, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x10));  // switch left up
    wheel.button(19, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x10)); // switch left down
    wheel.button(10, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x10)); // switch right up
    wheel.button(20, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x10)); // switch right down

    wheel.button(33, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x20)); // switch left up
    wheel.button(34, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x20)); // switch left down
    wheel.button(35, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x20)); // switch right up
    wheel.button(36, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x20)); // switch right down

    wheel.button(37, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x30)); // switch left up
    wheel.button(38, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x30)); // switch left down
    wheel.button(39, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x30)); // switch right up
    wheel.button(40, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x30)); // switch right down

    wheel.button(41, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x40)); // switch left up
    wheel.button(42, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x40)); // switch left down
    wheel.button(43, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x40)); // switch right up
    wheel.button(44, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x40)); // switch right down

    wheel.button(45, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x50)); // switch left up
    wheel.button(46, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x50)); // switch left down
    wheel.button(47, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x50)); // switch right up
    wheel.button(48, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x50)); // switch right down

    wheel.button(49, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x60)); // switch left up
    wheel.button(50, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x60)); // switch left down
    wheel.button(51, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x60)); // switch right up
    wheel.button(52, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x60)); // switch right down

    wheel.button(53, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x70)); // switch left up
    wheel.button(54, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x70)); // switch left down
    wheel.button(55, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x70)); // switch right up
    wheel.button(56, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x70)); // switch right down

    wheel.button(57, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x80)); // switch left up
    wheel.button(58, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x80)); // switch left down
    wheel.button(59, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x80)); // switch right up
    wheel.button(60, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x80)); // switch right down

    wheel.button(61, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0x90)); // switch left up
    wheel.button(62, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0x90)); // switch left down
    wheel.button(63, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0x90)); // switch right up
    wheel.button(64, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0x90)); // switch right down

    wheel.button(65, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0xA0)); // switch left up
    wheel.button(66, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0xA0)); // switch left down
    wheel.button(67, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0xA0)); // switch right up
    wheel.button(68, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0xA0)); // switch right down

    wheel.button(69, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0xB0)); // switch left up
    wheel.button(70, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0xB0)); // switch left down
    wheel.button(71, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0xB0)); // switch right up
    wheel.button(72, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0xB0)); // switch right down

    wheel.button(73, (mcl_in.buttons[1] & 0x10) && ((mcl_in.garbage[3] & 0xF0) == 0xC0)); // switch left up
    wheel.button(74, (mcl_in.buttons[2] & 0x80) && ((mcl_in.garbage[3] & 0xF0) == 0xC0)); // switch left down
    wheel.button(75, (mcl_in.buttons[1] & 0x02) && ((mcl_in.garbage[3] & 0xF0) == 0xC0)); // switch right up
    wheel.button(76, (mcl_in.buttons[2] & 0x40) && ((mcl_in.garbage[3] & 0xF0) == 0xC0)); // switch right down

    wheel.button(12, mcl_in.buttons[2] & 0x04); // hat button
    wheel.button(14, mcl_in.buttons[2] & 0x02); // xbox

    // paddles shitfer
    wheel.button(15, mcl_in.buttons[1] & 0x08); // left
    wheel.button(16, mcl_in.buttons[1] & 0x01); // right

    rotary_value = mcl_in.encoder;

    if ((mcl_in.garbage[2] & 0x0F) == 0x02 && !mcl_in.axisX) // left clutch fully pressed
    {
        clutch_max = constrain(clutch_max + rotary_value, 0, 0xFF);
    } else {
        wheel.button(17, rotary_value <= -1); // left
        wheel.button(18, rotary_value >= 1);  // right
    }

    // clutch paddle
    switch (mcl_in.garbage[2] & 0x0F) {
        case 0x01:
            // bite point
            wheel.doubleClutch(~mcl_in.axisX, ~mcl_in.axisY);
            break;
        case 0x02:
            // bite point advanced
            wheel.doubleClutch(map(~mcl_in.axisX & 0xFF, 0, 0xFF, 0, clutch_max), ~mcl_in.axisY & 0xff);
            break;
        default:
            wheel.doubleAxis(~mcl_in.axisX, ~mcl_in.axisY);
    }

    //wheel.button(11, mcl_in.buttons[2] & 0x20); // display button

    wheel.button(21, ((mcl_in.garbage[3] & 0x0f) == 0x01) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(22, ((mcl_in.garbage[3] & 0x0f) == 0x02) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(23, ((mcl_in.garbage[3] & 0x0f) == 0x03) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(24, ((mcl_in.garbage[3] & 0x0f) == 0x04) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(25, ((mcl_in.garbage[3] & 0x0f) == 0x05) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(26, ((mcl_in.garbage[3] & 0x0f) == 0x06) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(27, ((mcl_in.garbage[3] & 0x0f) == 0x07) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(28, ((mcl_in.garbage[3] & 0x0f) == 0x08) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(29, ((mcl_in.garbage[3] & 0x0f) == 0x09) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(30, ((mcl_in.garbage[3] & 0x0f) == 0x0A) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(31, ((mcl_in.garbage[3] & 0x0f) == 0x0B) && !((mcl_in.buttons[2] & 0x20) == 0x20));
    wheel.button(32, ((mcl_in.garbage[3] & 0x0f) == 0x0C) && !((mcl_in.buttons[2] & 0x20) == 0x20));

    wheel.stick(0, 0);

#ifdef HAS_DEBUG
    Serial.print("MCL_IN:");
    for (int i = 0; i < sizeof(mcl_in.raw); i++) {

        Serial.print(mcl_in.raw[i], HEX);
        Serial.print(":");
    }
    Serial.println();
#endif
}

void loop() {
#ifdef IS_USB
    // Fetching HID packet
    uint16_t hid_size;
    hid_size = Joystick.recv(&hid_pck, 0);
    if (hid_size > 0) {
        receive_hid_from_host(1, hid_size, hid_pck);
    }
#endif

    switch (detectWheelType()) {
        case CSW_WHEEL:
            csw_loop();
            break;
        case CSL_WHEEL:
            csl_loop();
            break;
        case MCL_WHEEL:
            mcl_loop();
            break;
        default:
            // no wheel  ?
            whClear();
            delay(10);
    }

// Send HID report (all inputs)
#ifdef IS_USB
    uint32_t now = micros();
    if (now >= usb_time) {
        uint32_t delta_t = now - usb_time;
#ifdef HAS_DEBUG
        Serial.println(String("usb loop time : ") + delta_t);
#endif

        if (delta_t >= MAX_SPEED) {
// delayMicroseconds(MAX_SPEED - delta_t);
#ifdef HAS_DEBUG
            Serial.println(String("usb loop time pre send: ") + (micros() - usb_time));
#endif
            Joystick.send_now();
#ifdef HAS_DEBUG
            Serial.println(String("usb loop time post send: ") + (micros() - usb_time));
#endif
            usb_time = micros();
        } else {
#ifdef HAS_DEBUG
            Serial.println(String("skipping report"));
#endif
        }
    }

    // rotary_debounce = 0;
#else
    uint32_t timout;
    timout = micros() - timing;
    if (timout > max_delay || (in_changed && timout > 10000))
    {
      // hid_data[3] = (hid_data[3]+1)&0xff;
      iwrap_send_data(main_link_id, sizeof(hid_data), hid_data, iwrap_mode);
      timing = micros();
#ifdef HAS_DEBUG
      if (rotary_debounce != 0)
        Serial.println(String("RESET!!!!!!!!!ROTARY : ") + rotary_value);
      if (in_changed)
        Serial.println("input sent");
#endif
      in_changed = false;
      // rotary_debounce = 0;
    }
#endif


#ifndef IS_USB
#endif
    // timing = micros() - timing;
    // if(timing > 550)  Serial.println(String("timing: ") + timing);
    // some delay
    idle();
}

void whClear() {
    wheel.clear();
    clutch_max = 0xFF;
    show_fwvers = true;
    fwShownForDuration = 0;
}

void receive_hid_from_host(uint8_t link_id, uint16_t data_length, const uint8_t *data) {
    if (data[2] == 0x01 && data[3] == 0x02) {
        // 7 seg
        if (detectWheelType() == MCL_WHEEL) {
            mcl_out.raw[1] = 0x11;
            mcl_out.raw[2] = csw7segToAscii(data[4] & 0xff);
            mcl_out.raw[3] = csw7segToAscii(data[5] & 0xff);
            mcl_out.raw[4] = csw7segToAscii(data[6] & 0xff);

#ifdef HAS_DEBUG
            Serial.print(String("HID display: "));
            Serial.print(data[4], HEX);
            Serial.print(":");
            Serial.print(data[5], HEX);
            Serial.print(":");
            Serial.print(data[6], HEX);
            Serial.println();
#endif
        } else {
            if (csw_in.id == CSLMCLGT3) {
                wheel._csw_out.raw[1] = 0x11;
                wheel._csw_out.raw[2] = csw7segToAscii(data[4] & 0xff);
                wheel._csw_out.raw[3] = csw7segToAscii(data[5] & 0xff);
                wheel._csw_out.raw[4] = csw7segToAscii(data[6] & 0xff);
            } else {
                wheel._csw_out.disp[0] = (data[4] & 0xff);
                wheel._csw_out.disp[1] = (data[5] & 0xff);
                wheel._csw_out.disp[2] = (data[6] & 0xff);
            }
#ifdef HAS_DEBUG
            Serial.println(
                    String("HID display: ") + wheel._csw_out.disp[0] + ":" + wheel._csw_out.disp[1] + ":" + wheel._csw_out.disp[2]);
#endif
        }
    } else if (data[2] == 0x01 && data[3] == 0x03) {
        // rumbles
        if (wheel._csw_out.id != UNIHUB && csw_in.id != CSLMCLGT3) {
            wheel._csw_out.rumble[0] = (data[4] & 0xff);
            wheel._csw_out.rumble[1] = (data[5] & 0xff);
        }
#ifdef HAS_DEBUG
        Serial.println(String("HID rumbles: ") + wheel._csw_out.rumble[0] + ":" + wheel._csw_out.rumble[1]);
#endif
    } else if (data[2] == 0x08) {
        // Rev Lights
        if (wheel._csw_out.id != UNIHUB && csw_in.id != CSLMCLGT3) {
            wheel._csw_out.leds = (data[3] & 0xff) << 8 | (data[4] & 0xff);
            // ftx_pck[5] = (hid_pck[4] & 0xff);
            // ftx_pck[6] = (hid_pck[3] & 0xff);
        }
#ifdef HAS_DEBUG
        Serial.println(String("HID leds   : ") + wheel._csw_out.leds);
#endif
    } else if (data[1] == 0x14) {
        // ??
    } else {

#ifdef HAS_DEBUG
        Serial.print(String("[!] HID Unknown from ") + link_id + " (size:" + data_length + ") : ");

        for (int i = 0; i < data_length; i++) {
            Serial.print(data[i], HEX);
            Serial.print(":");
        }
        Serial.println();
#endif
    }
    millis();
}

const unsigned char seven_seg_digits_decode_gfedcba[75] = {
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
        0x76, 0x30, 0x1E, 0x75, 0x38, 0x55, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
        0x78, 0x3E, 0x1C, 0x1D, 0x64, 0x6E, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
        0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, 0x1E, 0x75,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
        0x38, 0x55, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x1D,
/*  x     y     z     */
        0x64, 0x6E, 0x5B
};

int csw_ascii_to_7seg(char c) {
    if (c > (unsigned char) 'z')
        return 0x00;
    return seven_seg_digits_decode_gfedcba[c - '0'];
}

void show_firmware_version() {
    if (!show_fwvers)
        return;

    bool isFirstLoop = fwShownForDuration == 0;
    if (isFirstLoop) {
        Serial.println("Init wheel");
        // start showing fw vers
        if (detectWheelType() == MCL_WHEEL) {
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
        } else {
            String fw_vers = String(csw_in.fwvers);
            if (csw_in.id == CSLMCLGT3) {
                wheel._csw_out.raw[1] = 0x11;
                // Erase all
                wheel._csw_out.raw[2] = 0x0A;
                wheel._csw_out.raw[3] = 0x0A;
                wheel._csw_out.raw[4] = 0x0A;
                //
                wheel._csw_out.raw[2] = fw_vers.charAt(0);
                if (fw_vers.length() > 1)
                    wheel._csw_out.raw[3] = fw_vers.charAt(1);
                if (fw_vers.length() > 2)
                    wheel._csw_out.raw[4] = fw_vers.charAt(2);
            } else {
                String fw_vers = String(csw_in.fwvers);

                wheel._csw_out.disp[0] = 0;
                wheel._csw_out.disp[1] = 0;
                wheel._csw_out.disp[2] = 0;

                wheel._csw_out.disp[0] = csw_ascii_to_7seg(fw_vers.charAt(0));
                if (fw_vers.length() > 1)
                    wheel._csw_out.disp[1] = csw_ascii_to_7seg(fw_vers.charAt(1));
                if (fw_vers.length() > 2)
                    wheel._csw_out.disp[2] = csw_ascii_to_7seg(fw_vers.charAt(2));
            }
        }
        fwShownForDuration = millis();
    } else if (millis() - fwShownForDuration >= 4000) {
        show_fwvers = false;
        if (detectWheelType() == CSW_WHEEL) {
            wheel._csw_out.disp[0] = 0;
            wheel._csw_out.disp[1] = 0;
            wheel._csw_out.disp[2] = 0;
        } else {
            mcl_out.raw[1] = 0x11;
            mcl_out.raw[2] = 0x0A;
            mcl_out.raw[3] = 0x0A;
            mcl_out.raw[4] = 0x0A;
            mcl_out.raw[9] = 0x00;
            if (csw_in.id == CSLMCLGT3) {
                wheel._csw_out.raw[1] = 0x11;
                wheel._csw_out.raw[2] = 0x0A;
                wheel._csw_out.raw[3] = 0x0A;
                wheel._csw_out.raw[4] = 0x0A;
            }
        }
    }
}

void idle() {
#ifndef IS_USB
    delay(1);
#endif
}
