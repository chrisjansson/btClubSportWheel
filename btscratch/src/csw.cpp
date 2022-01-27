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
#include <fsm/States/DetectWheel.h>
#include "Arduino.h"
#include <bluefruit.h>
#include "JoystickHid.h"
#include "logger.h"

class csw_wheel csw_wheel;
class mcl_wheel mcl_wheel;
class csl_wheel csl_wheel;
WheelBase initial_wheel;
WheelBase *wheel = &initial_wheel;
WheelFsm fsm;

JoystickHid hidService = JoystickHid();
uint32_t serialStartupWait = 2000;

void setupSerial() {
    auto serialStartup = millis();

    Serial.begin(115200);

    auto elapsed = millis() - serialStartup;
    while (!Serial || elapsed < serialStartupWait) {
        delay(10);
    }

    if (Serial) {
        logger = &Serial;
    }
}


void setup() {
    fsetup();
    wheel->clear();
    setupSerial();
    fsm.setState(DetectWheel::create());

    logger->println("MCU Ready");
    for (int i = 0; i < 100; ++i) {
        delay(100);
        logger->print(".");
    }
}

void loop() {
    fsm.loop();
}