#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include "Arduino.h"
#include "joystick_report_builder.h"

class Joystick_T
{
  public:
    void send_now();

    void button(uint8_t button, bool val);

    void X(unsigned int val);

    void Y(unsigned int val);

    void clutch1(unsigned int val);

    void clutch2(unsigned int val);

    void hat(int val);

    void setWheel(unsigned int val);

  private:
    JoystickReportBuilder _joystickReportBuilder;
};

extern Joystick_T Joystick;

#endif