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
#include "fsm/WheelFsm.h"

class csw_wheel csw_wheel;
class mcl_wheel mcl_wheel;
class csl_wheel csl_wheel;
WheelBase initial_wheel;
WheelBase *wheel = &initial_wheel;

void setup2();

WheelFsm fsm;

void setup() {
    fsetup();
    wheel->clear();
    setup2();
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
}

void loop() {
    fsm.loop();
}