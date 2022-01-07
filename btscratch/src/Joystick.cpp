#include <services/BLEHidGeneric.h>
#include "Joystick.h"

extern BLEHidGeneric hidService;


void Joystick_T::button(uint8_t button, bool val)
{
    _joystickReportBuilder.setButton(button, val);
}

void Joystick_T::setWheel(unsigned int val)
{
    _joystickReportBuilder.setWheel(val);
}

void Joystick_T::X(unsigned int val)
{
    _joystickReportBuilder.setAxisX(val);
}

void Joystick_T::Y(unsigned int val)
{
    _joystickReportBuilder.setAxisY(val);

}

int Joystick_T::recv(void *buffer, uint16_t timeout)
{
  return 0;
}

void Joystick_T::clutch1(unsigned int val)
{
    _joystickReportBuilder.setClutch1(val);
}

void Joystick_T::clutch2(unsigned int val)
{
    _joystickReportBuilder.setClutch2(val);
}

void Joystick_T::hat(int val)
{
    _joystickReportBuilder.setHat(val);
}

void Joystick_T::send_now()
{
    //TODO: Check if report has changed before sending
    hidService.inputReport(_joystickReportBuilder.getReportId(), _joystickReportBuilder.getReport(), _joystickReportBuilder.getReportLength());
}

Joystick_T Joystick = Joystick_T();