#include "DisplayFirmware.h"
#include <Arduino.h>
#include "States.h"

#define VBATPIN A6

float readBatterVoltage() {
    float batteryVoltage = analogRead(VBATPIN);
    batteryVoltage *= 2;    // we divided by 2, so multiply back
    batteryVoltage *= 3.6;  // Multiply by 3.6V, our reference voltage
    batteryVoltage /= 1024; // convert to voltage
    return batteryVoltage;
}


void DisplayFirmware::enter(WheelFsm *fsm) {
    _startTime = millis();
    _voltage = readBatterVoltage();
}

void DisplayFirmware::loop(WheelFsm *fsm) {
    if (_startTime + 2000 >= millis()) {
        wheel->display_firmware_version();
        wheel->transferData();
        return;
    }

    if (_startTime + 4000 >= millis()) {
        wheel->display_battery_voltage(_voltage);
        wheel->transferData();

        return;
    }

    auto nextState = ConnectBle::create(wheel);
    fsm->setState(nextState);
}

void DisplayFirmware::exit(WheelFsm *fsm) {
    wheel->clear_display();
    wheel->transferData();
}

DisplayFirmware *DisplayFirmware::create(WheelBase *wheel) {
    DisplayFirmware::instance = DisplayFirmware(wheel);
    return &DisplayFirmware::instance;
}

DisplayFirmware::DisplayFirmware(WheelBase *wheel) : wheel(wheel) {

}

DisplayFirmware DisplayFirmware::instance = nullptr;

const char *DisplayFirmware::getName() {
    return "Display firmware";
}
