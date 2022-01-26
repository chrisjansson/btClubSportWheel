#include <Arduino.h>
#include <bluefruit.h>
#include "joystick_report_builder.h"
#include "JoystickHid.h"
#include "logger.h"

JoystickHid hidService = JoystickHid();
Print *logger = &nullLogger;

void setup2() {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);   // for nrf52840 with native usb
    }

    logger = &Serial;
}