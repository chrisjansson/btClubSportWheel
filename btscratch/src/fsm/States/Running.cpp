//
// Created by chris on 2022-01-05.
//

#include "Running.h"
#include <Arduino.h>

static uint32_t timing;
static uint32_t usb_time;

#define MAX_SPEED 25000

static void idle();

static void send_hid_report() {// Send HID report (all inputs)
#ifdef IS_USB
//TODO: Check for overflow 70min
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

static void idle() {
#ifndef IS_USB
    delay(1);
#endif
}


Running::Running(WheelBase *wheel) : wheel(wheel) {

}

void Running::enter(WheelFsm *fsm) {
    timing = micros();
    usb_time = micros();
}

uint8_t hid_pck[7];

void Running::loop(WheelFsm *fsm) {
    uint16_t hid_size;
    hid_size = Joystick.recv(&hid_pck, 0);
    if (hid_size > 0) {
        wheel->receive_hid_from_host(1, hid_size, hid_pck);
    }

    wheel->transferData();
    wheel->process();
    send_hid_report();
}

Running *Running::create(WheelBase *wheel) {
    Running::instance = Running(wheel);
    return &Running::instance;
}

Running Running::instance = nullptr;


