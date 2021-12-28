#include <services/BLEHidGeneric.h>
#include "Joystick.h"

extern BLEHidGeneric hidService;


void Joystick_T::button(uint8_t button, bool val)
{
    _joystickReportBuilder.setButton(button, val);
}

void Joystick_T::useManualSend(bool useManualSend)
{
}

void Joystick_T::setWheel(unsigned int val)
{
}

void Joystick_T::X(unsigned int val)
{
}

void Joystick_T::Y(unsigned int val)
{
}

int Joystick_T::recv(void *buffer, uint16_t timeout)
{
  return 0;
}

void Joystick_T::clutch1(unsigned int val)
{
}

void Joystick_T::clutch2(unsigned int val)
{
}

void Joystick_T::send_now()
{
    hidService.inputReport(_joystickReportBuilder.getReportId(), _joystickReportBuilder.getReport(), _joystickReportBuilder.getReportLength());
//    delay(50);
//    _joystickReportBuilder.clear();

}

void Joystick_T::hat(int val)
{
}

Joystick_T Joystick = Joystick_T();