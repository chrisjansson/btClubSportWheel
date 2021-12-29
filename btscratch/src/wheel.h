//
// Created by chris on 2021-12-29.
//

#ifndef BTSCRATCH_WHEEL_H
#define BTSCRATCH_WHEEL_H


class wheel {
public:
    wheel(){
        // debounce timer for rotary encoder
        _buttonDebouncers[17].interval(30);
        _buttonDebouncers[18].interval(30);
    }

    void whButton(uint8_t button, bool val) {
        val = _buttonDebouncers[button].get(val);
        Joystick.button(button, val);
    }

private:
    Debouncer* _buttonDebouncers = new Debouncer[89];
};


#endif //BTSCRATCH_WHEEL_H
