/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include <Arduino.h>
#include <bluefruit.h>
#include "manifest.h"
#include "joystick_report_builder.h"

BLEHidGeneric hidService = BLEHidGeneric(1, 1, 0);

void setup2()
{
    Serial.begin(115200);
    while ( !Serial ) delay(10);   // for nrf52840 with native usb

    Serial.println("Bluefruit52 HID Keyboard Example");
    Serial.println("--------------------------------\n");

    Serial.println();
    Serial.println("Go to your phone's Bluetooth settings to pair your device");
    Serial.println("then open an application that accepts keyboard input");

    Serial.println();
    Serial.println("Enter the character(s) to send:");
    Serial.println();
}
//
//int loops = 0;
//int battery = 0;
//
////uint8_t data[3];
//uint8_t buttons[32];
//
//uint8_t button = 0;

void loop2()
{
//    buttons[0] += 1;
//    buttons[1] += 1;
//    buttons[2] += 1;
//    buttons[3] += 1;
//    buttons[4] += 1;
//    buttons[5] += 1;
//    buttons[6] += 1;
//    buttons[7] += 1;
//    buttons[8] += 1;
//    buttons[9] += 1;
//    buttons[10] += 1;
//    buttons[11] += 1;
//    buttons[12] += 1;
    // Only send KeyRelease if previously pressed to avoid sending
    // multiple keyRelease reports (that consume memory and bandwidth)
//    if ( hasKeyPressed )
//    {
//        hasKeyPressed = false;
//        blehid.keyRelease();

        // Delay a bit after a report
//        delay(5);
//    }

//        char ch = (char) Serial.read();

        // echo
//        Serial.write(ch);

//        blehid.keyPress(ch);
//        hasKeyPressed = true;

        // Delay a bit after a report
//    delay(5);
//    loops++;
//    if(loops > 1000){
//        loops = 0;
//        battery = (battery + 1) % 1000;
//        blebatc.notify8(battery);
//        Serial.println("Set battery");
//
//    }

//    hidService.inputReport(1, data, 3);
//
//    joystickReportBuilder.setButton(button, 1);
//    joystickReportBuilder.setHat(button % 9);
//    joystickReportBuilder.setAxisX(128);
//    joystickReportBuilder.setAxisY(128);
//    joystickReportBuilder.setClutch1(30);
//    joystickReportBuilder.setClutch2(200);
//
//    hidService.inputReport(joystickReportBuilder.getReportId(), joystickReportBuilder.getReport(), joystickReportBuilder.getReportLength());
//    delay(500);
//    joystickReportBuilder.clear();
//    button += 1;
//    button = button % 88;

//    blebat.setBatteryLevel(button);
    //    delay(5);
}
