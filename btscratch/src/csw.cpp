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
#include <wheels/csw_wheel.h>
#include <wheels/mcl_wheel.h>
#include <wheels/csl_wheel.h>
#include "Arduino.h"

#include "fanatec.h"
#include "Debouncer.h"
#include "Joystick.h"
#include "wheels/wheel.h"

#define MAX_SPEED 50000

uint8_t hid_data[35];

bool in_changed;

void setup();

void setup2();

void loop();

void idle();

void whClear();

uint32_t timing;
uint32_t usb_time;

uint8_t hid_pck[7];

csw_wheel csw_wheel;
mcl_wheel mcl_wheel;
csl_wheel csl_wheel;
wheel initial_wheel;

wheel* wheel = &initial_wheel;

uint8_t main_link_id = 1;

void setup() {
    fsetup();

    whClear();

    // prebuild output packet
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

void loop() {
#ifdef IS_USB
    // Fetching HID packet
    uint16_t hid_size;
    hid_size = Joystick.recv(&hid_pck, 0);
    if (hid_size > 0) {
        wheel->receive_hid_from_host(1, hid_size, hid_pck);
    }
#endif

    switch (detectWheelType()) {
        case CSW_WHEEL:
            wheel = &csw_wheel;
            wheel->transferData();
            wheel->show_firmware_version();
            wheel->process();
            break;
        case CSL_WHEEL:
            wheel = &csl_wheel;
            wheel->transferData();
            wheel->show_firmware_version();
            wheel->process();
            break;
        case MCL_WHEEL:
            wheel = &mcl_wheel;
            wheel->transferData();
            wheel->show_firmware_version();
            wheel->process();
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

    // timing = micros() - timing;
    // if(timing > 550)  Serial.println(String("timing: ") + timing);
    // some delay
    idle();
}

void whClear() {
    wheel->clear();
}

void idle() {
#ifndef IS_USB
    delay(1);
#endif
}
