#ifndef BTSCRATCH_JOYSTICK_REPORT_BUILDER_H
#define BTSCRATCH_JOYSTICK_REPORT_BUILDER_H

#include <Arduino.h>


/// Report builder for report descriptor in joystick_report_descriptor.h
class JoystickReportBuilder {
public:
    void setButton(uint8_t button, uint8_t value);
    void setHat(uint8_t value);
    void setAxisX(uint8_t value);
    void setAxisY(uint8_t value);
    void setClutch1(uint8_t value);
    void setClutch2(uint8_t value);

    void clear();
    uint8_t* getReport();

private:
    uint8_t _report[15];
};

#endif //BTSCRATCH_JOYSTICK_REPORT_BUILDER_H
