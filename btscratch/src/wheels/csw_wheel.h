//
// Created by chris on 2021-12-29.
//

#ifndef BTSCRATCH_CSW_WHEEL_H
#define BTSCRATCH_CSW_WHEEL_H

#include "WheelBase.h"
#include "../fanatec.h"

class csw_wheel : public WheelBase {
public:
    csw_wheel() {
        _csw_out.header = 0xa5;
        _csw_out.id = 0x00;
    }

    void transferData() override {
        // csw stuff
        // Read Fanatec Packet
        //csw_out.raw[9] = 0x0F; // xbox light
        transferCswData(&_csw_out, &_csw_in, sizeof(_csw_out.raw));
    }

    void set_7seg(uint8_t a, uint8_t b, uint8_t c) override {
        if (_csw_in.id == CSLMCLGT3) {
            _csw_out.raw[1] = 0x11;
            _csw_out.raw[2] = csw7segToAscii(a);
            _csw_out.raw[3] = csw7segToAscii(b);
            _csw_out.raw[4] = csw7segToAscii(c);
        } else {
            _csw_out.disp[0] = a;
            _csw_out.disp[1] = b;
            _csw_out.disp[2] = c;
        }

#ifdef HAS_DEBUG
        Serial.println(                    String("HID display: ") + _csw_out.disp[0] + ":" + _csw_out.disp[1] + ":" + _csw_out.disp[2]);
#endif
    }

    void set_lights(uint8_t a, uint8_t b) override {
        if (_csw_out.id != UNIHUB && _csw_in.id != CSLMCLGT3) {
            _csw_out.leds = (a & 0xff) << 8 | (b & 0xff);
        }
#ifdef HAS_DEBUG
        Serial.println(String("HID leds   : ") + _csw_out.leds);
#endif
    }

    void process() override {
#ifdef HAS_DEBUG
        Serial.print("CSW_IN:");
    for (int i = 0; i < sizeof(_csw_in.raw); i++) {

        Serial.print(_csw_in.raw[i], HEX);
        Serial.print(":");
    }
    Serial.println();
#endif

        // Wheel ID
        setId(_csw_in.id);
        _csw_out.id = _csw_in.id & 0xFF;

        // Left stick
        stick(_csw_in.axisX, _csw_in.axisY);

        // All buttons
        button(1, _csw_in.buttons[0] & 0x80);  // first top right
        button(2, _csw_in.buttons[0] & 0x40);  // middle right
        button(3, _csw_in.buttons[0] & 0x20);  // second top right
        button(4, _csw_in.buttons[0] & 0x10);  // bottom right
        button(5, _csw_in.buttons[1] & 0x80);  // third center
        button(6, _csw_in.buttons[1] & 0x40);  // first center
        button(7, _csw_in.buttons[1] & 0x20);  // middle left
        button(9, _csw_in.buttons[1] & 0x04);  // bottom left
        button(11, _csw_in.buttons[2] & 0x08); // second center
        button(12, _csw_in.buttons[2] & 0x04); // stick button

        // paddles shitfer

        uint8_t left_paddle = 0;
        uint8_t right_paddle = 0;

        left_paddle = _csw_in.buttons[1] & 0x08;
        right_paddle = _csw_in.buttons[1] & 0x01;

        if (_csw_in.id == XBOXHUB) {
            left_paddle |= _csw_in.garbage[0] & 0x02;
            right_paddle |= _csw_in.garbage[0] & 0x08;
        }

        button(15, left_paddle); // left
        button(16, right_paddle); // right

        int8_t rotary_value = 0;
        rotary_value = _csw_in.encoder;

        if (_csw_in.id != CSLMCLGT3) {
            button(8, _csw_in.buttons[1] & 0x10);  // first top left
            button(10, _csw_in.buttons[1] & 0x02); // second top left
            button(13, _csw_in.buttons[2] & 0x02); // hat button
            button(14, _csw_in.buttons[2] & 0x20); // display button

            button(17, rotary_value == -1); // left
            button(18, rotary_value == 1);  // right
        }

        if (_csw_in.id == UNIHUB || _csw_in.id == XBOXHUB) {
            // Uni Hub extra buttons
            // BUT_5 array (optional 3 buttons)
            button(19, _csw_in.btnHub[0] & 0x08);
            button(20, _csw_in.btnHub[0] & 0x10);
            button(21, _csw_in.btnHub[0] & 0x20);

            // Playstation buttons
            button(22, _csw_in.btnPS[0] & 0x01);
            button(23, _csw_in.btnPS[0] & 0x02);
            button(24, _csw_in.btnPS[0] & 0x04);
            button(25, _csw_in.btnPS[0] & 0x08);
            button(26, _csw_in.btnPS[0] & 0x10);
            button(27, _csw_in.btnPS[0] & 0x20);
            button(28, _csw_in.btnPS[0] & 0x40);
            button(29, _csw_in.btnPS[0] & 0x80);

            button(30, _csw_in.btnPS[1] & 0x01);
            button(31, _csw_in.btnPS[1] & 0x02);
            button(32, _csw_in.btnPS[1] & 0x04);
            button(33, _csw_in.btnPS[1] & 0x08);
            button(34, _csw_in.btnPS[1] & 0x10);
            button(35, _csw_in.btnPS[1] & 0x20);
            button(36, _csw_in.btnPS[1] & 0x40);
            button(37, _csw_in.btnPS[1] & 0x80);

        }

        if (_csw_in.id == XBOXHUB) {
            // Xbox Hub has 1 extra button
            button(38, _csw_in.btnHub[1] & 0x08);

            //APM
            button(39, _csw_in.garbage[0] & 0x01);
            // button(40, _csw_in.garbage[0] & 0x02);
            button(41, _csw_in.garbage[0] & 0x04);
            // button(42, _csw_in.garbage[0] & 0x08);
            doubleAxis(_csw_in.garbage[1], _csw_in.garbage[2]);
        }

        hat(_csw_in.buttons[0] & 0x0f, false);

        // Serial.println(String("button: ") + hid_data[3]);

        if (_csw_in.id == CSLMCLGT3) {
            button(8, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x10));  // switch left up
            button(19,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x10)); // switch left down
            button(10, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x10)); // switch right up
            button(20,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x10)); // switch right down

            button(33, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x20)); // switch left up
            button(34,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x20)); // switch left down
            button(35, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x20)); // switch right up
            button(36,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x20)); // switch right down

            button(37, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x30)); // switch left up
            button(38,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x30)); // switch left down
            button(39, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x30)); // switch right up
            button(40,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x30)); // switch right down

            button(41, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x40)); // switch left up
            button(42,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x40)); // switch left down
            button(43, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x40)); // switch right up
            button(44,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x40)); // switch right down

            button(45, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x50)); // switch left up
            button(46,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x50)); // switch left down
            button(47, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x50)); // switch right up
            button(48,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x50)); // switch right down

            button(49, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x60)); // switch left up
            button(50,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x60)); // switch left down
            button(51, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x60)); // switch right up
            button(52,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x60)); // switch right down

            button(53, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x70)); // switch left up
            button(54,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x70)); // switch left down
            button(55, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x70)); // switch right up
            button(56,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x70)); // switch right down

            button(57, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x80)); // switch left up
            button(58,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x80)); // switch left down
            button(59, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x80)); // switch right up
            button(60,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x80)); // switch right down

            button(61, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0x90)); // switch left up
            button(62,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0x90)); // switch left down
            button(63, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0x90)); // switch right up
            button(64,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0x90)); // switch right down

            button(65, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0xA0)); // switch left up
            button(66,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0xA0)); // switch left down
            button(67, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0xA0)); // switch right up
            button(68,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0xA0)); // switch right down

            button(69, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0xB0)); // switch left up
            button(70,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0xB0)); // switch left down
            button(71, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0xB0)); // switch right up
            button(72,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0xB0)); // switch right down

            button(73, (_csw_in.buttons[1] & 0x10) && ((_csw_in.garbage[3] & 0xF0) == 0xC0)); // switch left up
            button(74,
                   (_csw_in.buttons[2] & 0x80) && ((_csw_in.garbage[3] & 0xF0) == 0xC0)); // switch left down
            button(75, (_csw_in.buttons[1] & 0x02) && ((_csw_in.garbage[3] & 0xF0) == 0xC0)); // switch right up
            button(76,
                   (_csw_in.buttons[2] & 0x40) && ((_csw_in.garbage[3] & 0xF0) == 0xC0)); // switch right down

            button(14, _csw_in.buttons[2] & 0x02); // xbox

            if ((_csw_in.garbage[2] & 0x0F) == 0x02 && !_csw_in.axisX) // left clutch fully pressed
            {
                clutch_max = constrain(clutch_max + rotary_value, 0, 0xFF);
            } else {
                button(17, rotary_value <= -1); // left
                button(18, rotary_value >= 1);  // right
            }

            // clutch paddle
            switch (_csw_in.garbage[2] & 0x0F) {
                case 0x01:
                    // bite point
                    doubleClutch(~_csw_in.axisX, ~_csw_in.axisY);
                    break;
                case 0x02:
                    // bite point advanced
                    doubleClutch(map(~_csw_in.axisX & 0xFF, 0, 0xFF, 0, clutch_max), ~_csw_in.axisY & 0xff);
                    break;
                default:
                    doubleAxis(~_csw_in.axisX, ~_csw_in.axisY);
            }

            //button(11, mcl_in.buttons[2] & 0x20); // display button

            button(21, ((_csw_in.garbage[3] & 0x0f) == 0x01) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(22, ((_csw_in.garbage[3] & 0x0f) == 0x02) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(23, ((_csw_in.garbage[3] & 0x0f) == 0x03) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(24, ((_csw_in.garbage[3] & 0x0f) == 0x04) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(25, ((_csw_in.garbage[3] & 0x0f) == 0x05) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(26, ((_csw_in.garbage[3] & 0x0f) == 0x06) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(27, ((_csw_in.garbage[3] & 0x0f) == 0x07) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(28, ((_csw_in.garbage[3] & 0x0f) == 0x08) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(29, ((_csw_in.garbage[3] & 0x0f) == 0x09) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(30, ((_csw_in.garbage[3] & 0x0f) == 0x0A) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(31, ((_csw_in.garbage[3] & 0x0f) == 0x0B) && !((_csw_in.buttons[2] & 0x20) == 0x20));
            button(32, ((_csw_in.garbage[3] & 0x0f) == 0x0C) && !((_csw_in.buttons[2] & 0x20) == 0x20));

            stick(0, 0);
        }
    }

    void clearBase() override {
        clutch_max = 0xFF;
    }

    void set_rumble(uint8_t a, uint8_t b) override {
        if (_csw_out.id != UNIHUB && _csw_in.id != CSLMCLGT3) {
            _csw_out.rumble[0] = (a & 0xff);
            _csw_out.rumble[1] = (b & 0xff);
        }
    }

    void clear_display() override {
        if (_csw_in.id == CSLMCLGT3) {
            _csw_out.raw[1] = 0x11;
            _csw_out.raw[2] = 0x0A;
            _csw_out.raw[3] = 0x0A;
            _csw_out.raw[4] = 0x0A;
        } else {
            _csw_out.disp[0] = 0;
            _csw_out.disp[1] = 0;
            _csw_out.disp[2] = 0;
        }
    }

    void display_firmware_version() override {
        String firmware_version = String(_csw_in.fwvers);
        if (_csw_in.id == CSLMCLGT3) {
            _csw_out.raw[1] = 0x11;
            // Erase all
            _csw_out.raw[2] = 0x0A;
            _csw_out.raw[3] = 0x0A;
            _csw_out.raw[4] = 0x0A;
            //
            _csw_out.raw[2] = firmware_version.charAt(0);
            if (firmware_version.length() > 1)
                _csw_out.raw[3] = firmware_version.charAt(1);
            if (firmware_version.length() > 2)
                _csw_out.raw[4] = firmware_version.charAt(2);
        } else {
            _csw_out.disp[0] = 0;
            _csw_out.disp[1] = 0;
            _csw_out.disp[2] = 0;

            _csw_out.disp[0] = csw_ascii_to_7seg(firmware_version.charAt(0));
            if (firmware_version.length() > 1)
                _csw_out.disp[1] = csw_ascii_to_7seg(firmware_version.charAt(1));
            if (firmware_version.length() > 2)
                _csw_out.disp[2] = csw_ascii_to_7seg(firmware_version.charAt(2));
        }
    }

    uint8_t clutch_max = 0xFF;
    csw_in_t _csw_in;
    csw_out_t _csw_out;

    const unsigned char seven_seg_digits_decode_gfedcba[75] = {
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
            0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
            0x76, 0x30, 0x1E, 0x75, 0x38, 0x55, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
            0x78, 0x3E, 0x1C, 0x1D, 0x64, 0x6E, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
            0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 0x76, 0x30, 0x1E, 0x75,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
            0x38, 0x55, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x1D,
/*  x     y     z     */
            0x64, 0x6E, 0x5B
    };

    int csw_ascii_to_7seg(char c) {
        if (c > (unsigned char) 'z')
            return 0x00;
        return seven_seg_digits_decode_gfedcba[c - '0'];
    }
};


#endif //BTSCRATCH_CSW_WHEEL_H
