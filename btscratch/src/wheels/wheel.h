//
// Created by chris on 2021-12-29.
//

#ifndef BTSCRATCH_WHEEL_H
#define BTSCRATCH_WHEEL_H

#include "Joystick.h"
#include "Debouncer.h"
#include "fanatec.h"

class wheel {
public:
    wheel() {
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
        _csw_out.id = val & 0xFF;
        Joystick.setWheel(val);
    }

    void clear() {
        setId(NO_RIM);
        stick(0, 0);
        hat(0, false);
        doubleAxis(0x00, 0x00);
        //TODO: 37 looks supect, there are 89 debouncers
        for (int i = 1; i <= 37; ++i) {
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

    csw_out_t _csw_out;

private:
    Debouncer _hatDebouncer = Debouncer();
    Debouncer *_buttonDebouncers = new Debouncer[89];
};


#endif //BTSCRATCH_WHEEL_H
