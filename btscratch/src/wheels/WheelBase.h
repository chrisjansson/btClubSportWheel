//
// Created by chris on 2021-12-29.
//

#ifndef BTSCRATCH_WHEELBASE_H
#define BTSCRATCH_WHEELBASE_H

#include "Joystick.h"
#include "Debouncer.h"
#include "fanatec.h"

class WheelBase {
public:
    WheelBase() {
        // debounce timer for rotary encoder
        _buttonDebouncers[17].interval(30);
        _buttonDebouncers[18].interval(30);

        // debounce timer for hat switch
        _hatDebouncer.interval(50);
    }

    void button(uint8_t button, bool val) {
        val = _buttonDebouncers[button].get(val);
        Joystick.button(button, val);
    }

    void stick(unsigned int x, unsigned int y) {
        x = 255 - (x + 127);
        y = y + 127;
        Joystick.X(x);
        Joystick.Y(y);
    }

    void doubleAxis(unsigned int x, unsigned int y) {
        Joystick.clutch1(x);
        Joystick.clutch2(y);
    }

    void doubleClutch(unsigned int x, unsigned int y) {
        if (y > x)
            x = y;

        Joystick.clutch1(x);
        Joystick.clutch2(0);
    }

    void hat(int8_t val, bool is_csl) {
        val = _hatDebouncer.get(val);
        if (is_csl) { // CSL
            switch (val) {
                case 4:
                    val = 0;
                    break;
                case 2:
                    val = 2;
                    break;
                case 8:
                    val = 4;
                    break;
                case 16:
                    val = 6;
                    break;
                default:
                    val = 0xFF;
            }
        } else { // CSW
            switch (val) {
                case 1:
                    val = 0;
                    break;
                case 2:
                    val = 6;
                    break;
                case 4:
                    val = 2;
                    break;
                case 8:
                    val = 4;
                    break;
                default:
                    val = 0xFF;
            }
        }
        Joystick.hat(val);
    }

    void setId(unsigned int val) {
        Joystick.setWheel(val);
    }

    void clear() {
        setId(NO_RIM);
        stick(0, 0);
        hat(0, false);
        doubleAxis(0x00, 0x00);
        for (int i = 1; i < 88; ++i) {
            button(i, 0);
        }
        clearBase();
    }

    virtual void transferData() {

    }

    virtual void process() {

    }

    virtual void clearBase() {

    }

    virtual void set_rumble(uint8_t a, uint8_t b) {

    }

    virtual void set_7seg(uint8_t a, uint8_t b, uint8_t c) {

    }

    virtual void set_lights(uint8_t a, uint8_t b) {

    }

    void receive_hid_from_host(uint8_t link_id, uint16_t data_length, const uint8_t *data) {
        if (data[2] == 0x01 && data[3] == 0x02) {
            set_7seg(data[4] & 0xff, data[5] & 0xff, data[6] & 0xff);

            // 7 seg
            if (detectWheelType() == MCL_WHEEL) {
//                mcl_out.raw[1] = 0x11;
//                mcl_out.raw[2] = csw7segToAscii(data[4] & 0xff);
//                mcl_out.raw[3] = csw7segToAscii(data[5] & 0xff);
//                mcl_out.raw[4] = csw7segToAscii(data[6] & 0xff);

#ifdef HAS_DEBUG
                Serial.print(String("HID display: "));
            Serial.print(data[4], HEX);
            Serial.print(":");
            Serial.print(data[5], HEX);
            Serial.print(":");
            Serial.print(data[6], HEX);
            Serial.println();
#endif
            }
        } else if (data[2] == 0x01 && data[3] == 0x03) {
            set_rumble((data[4] & 0xff), (data[5] & 0xff));
#ifdef HAS_DEBUG
            Serial.println(String("HID rumbles: ") + WheelBase._csw_out.rumble[0] + ":" + WheelBase._csw_out.rumble[1]);
#endif
        } else if (data[2] == 0x08) {
            set_lights(data[3] & 0xff, data[4] & 0xff);
        } else if (data[1] == 0x14) {
            // ??
        } else {

#ifdef HAS_DEBUG
            Serial.print(String("[!] HID Unknown from ") + link_id + " (size:" + data_length + ") : ");

        for (int i = 0; i < data_length; i++) {
            Serial.print(data[i], HEX);
            Serial.print(":");
        }
        Serial.println();
#endif
        }
        millis();
    }

    virtual void clear_display() {

    }

    virtual void display_firmware_version() {

    }

private:
    Debouncer _hatDebouncer = Debouncer();
    Debouncer *_buttonDebouncers = new Debouncer[89];
};


#endif //BTSCRATCH_WHEELBASE_H
