#include "joystick_report_builder.h"

#include <Arduino.h>
#define __ASSERT_USE_STDERR
#include <assert.h>

void JoystickReportBuilder::setButton(uint8_t button, uint8_t value){
    assert(button >= 0 && button < 88);

    uint8_t bit = button % 8;
    uint8_t index = button / 8;
    uint8_t mask = ~(0x01 << bit);

    _report[index] = (_report[index] & mask) | ((value & 0x01) << bit);
}

void JoystickReportBuilder::setAxisX(uint8_t value) {
    _report[11] = value;
}

void JoystickReportBuilder::setAxisY(uint8_t value) {
    _report[12] = value;
}

void JoystickReportBuilder::setClutch1(uint8_t value) {
    _report[13] = value;
}

void JoystickReportBuilder::setClutch2(uint8_t value) {
    _report[14] = value;
}

void JoystickReportBuilder::setHat(uint8_t value) {
    _report[15] = value;
}

void JoystickReportBuilder::clear() {
    memclr(_report, sizeof  _report);
}

uint8_t* JoystickReportBuilder::getReport() {
    return _report;
}