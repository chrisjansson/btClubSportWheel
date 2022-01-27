#ifndef BTSCRATCH_JOYSTICK_REPORT_DESCRIPTOR_H
#define BTSCRATCH_JOYSTICK_REPORT_DESCRIPTOR_H

#include <Arduino.h>

#define JOYSTICK_REPORT_ID 0
#define JOYSTICK_INPUT_REPORT_SIZE 32
#define JOYSTICK_OUTPUT_REPORT_SIZE 8

extern uint8_t joystick_report_desc[];
extern size_t joystick_report_desc_size;

#endif //BTSCRATCH_JOYSTICK_REPORT_DESCRIPTOR_H
