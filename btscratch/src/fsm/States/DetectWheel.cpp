//
// Created by chris on 2022-01-05.
//

#include "DetectWheel.h"
#include "../../fanatec.h"
#include "States.h"
#include "../../logger.h"

Adafruit_NeoPixel pixel
= Adafruit_NeoPixel(1, PIN_NEOPIXEL);

void DetectWheel::loop(WheelFsm *fsm) {
    wheel_type wheelT = detectWheelType();
    if (wheelT == NO_WHEEL) {
        return;
    }

    WheelBase *wheel;
    switch (wheelT) {
        case CSW_WHEEL:
            wheel = &csw_wheel;
            break;
        case CSL_WHEEL:
            wheel = &csl_wheel;
            break;
        case MCL_WHEEL:
            wheel = &mcl_wheel;
            break;
        default:
            wheel = &initial_wheel;
            break;
    }

    wheel->clear();

    char s[40];
    sprintf(s, "Found wheel %d", wheelT);
    logger->println(s);

    pixel.setPixelColor(0, 0, 0, 255);
    pixel.show();

    fsm->setState(DisplayFirmware::create(wheel));
}

DetectWheel DetectWheel::instance = DetectWheel();

DetectWheel *DetectWheel::create() {
    return &DetectWheel::instance;
}

void DetectWheel::enter(WheelFsm *fsm) {
    logger->println("Detecting wheel...");
    pixel.begin();
    pixel.setBrightness(25);
    pixel.show();
    pixel.setPixelColor(0, 255, 0, 0);
    pixel.show();
}
