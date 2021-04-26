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

BLEDis bledis;

BLEService blebat = BLEService(UUID16_SVC_BATTERY);
BLECharacteristic blebatc = BLECharacteristic(UUID16_CHR_BATTERY_LEVEL);

BLEHidGeneric hidService = BLEHidGeneric(1, 1, 0);

static uint8_t joystick_report_desc[] = {

        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x04,                    // USAGE (Joystick)
        0xa1, 0x01,                    // COLLECTION (Application)

        // 88 Buttons (88bits)
        0x05, 0x09,                    //   USAGE_PAGE (Button)
        0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
        0x29, 0x58,                    //   USAGE_MAXIMUM (Button 88)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
        0x75, 0x01,                    //   REPORT_SIZE (1)
        0x95, 0x58,                    //   REPORT_COUNT (88)
        0x81, 0x02,                    //   INPUT (Data,Var,Abs)
        //   no padding ( 88 )

        // left stick axis (16bits)
        0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
        0x09, 0x30,                    //   USAGE (X)
        0x09, 0x31,                    //   USAGE (Y)
        0x09, 0x33,                    //   USAGE (Clutch)
        0x09, 0x34,                    //   USAGE (Clutch)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
        0x75, 0x08,                    //   REPORT_SIZE (8)
        0x95, 0x04,                    //   REPORT_COUNT (4)
        0x81, 0x02,                    //   INPUT (Data,Var,Abs)
        //   no padding ( 16 )


        // Hat switch (4bits)
        0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
        0x09, 0x39,                    //   USAGE (Hat)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
        0x35, 0x00,                    //   Physical Minimum (0)
        0x46, 0x3B, 0x01,              //   Physical Maximum (315)
        0x65, 0x14,                    //   Unit (20)
        0x75, 0x04,                    //   REPORT_SIZE (4)
        0x95, 0x01,                    //   REPORT_COUNT (1)
        0x81, 0x42,                    //   INPUT (Data,Var,Abs)
        //   padding ( total 124 -> (-256) 132 (4x33) )
        0x95, 0x21,                    //   REPORT_COUNT (33)
        0x75, 0x04,                    //   REPORT_SIZE (4)
        0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)

        // Total size : 256bits -> 32bytes (JOYSTICK_SIZE)

        // 4 LEDs

        0x05, 0x0a,                    //   USAGE_PAGE (Ordinals)
        0x09, 0x01,                    //   USAGE (Instance 1)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
        0xa1, 0x02,                    //   COLLECTION (Logical)
        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
        0x09, 0x4b,                    //     USAGE (Generic Indicator)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
        0xc0,                          //   END_COLLECTION
        0x09, 0x02,                    //   USAGE (Instance 2)
        0xa1, 0x02,                    //   COLLECTION (Logical)
        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
        0x09, 0x4b,                    //     USAGE (Generic Indicator)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
        0xc0,                          //   END_COLLECTION
        0x09, 0x03,                    //   USAGE (Instance 3)
        0xa1, 0x02,                    //   COLLECTION (Logical)
        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
        0x09, 0x4b,                    //     USAGE (Generic Indicator)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
        0xc0,                          //   END_COLLECTION
        0x09, 0x04,                    //   USAGE (Instance 4)
        0xa1, 0x02,                    //   COLLECTION (Logical)
        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
        0x09, 0x4b,                    //     USAGE (Generic Indicator)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
        0xc0,                          //   END_COLLECTION
        //   padding ( 4 + 4 )
        0x95, 0x0d,                    //   REPORT_COUNT (13)
        0x75, 0x04,                    //   REPORT_SIZE (4)
        0x91, 0x01,                    // OUTPUT (Cnst,Ary,Abs)
        //  no padding ( 64 )
        0xc0                           // END_COLLECTION

        /*
        0x05, 0x0a,                    //   USAGE_PAGE (Ordinals)
        0x09, 0x01,                    //   USAGE (Instance 1)
        0xa1, 0x01,                    //   COLLECTION (Application)
        //0x85, 0x01,                      // REPORT_ID (1)
        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
        0x09, 0x4b,                    //     USAGE (Generic Indicator)
        0x75, 0x01,                    //     REPORT_SIZE (8)
        0x95, 0x07,                    //     REPORT_COUNT (7)
        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
        0xc0,                          //   END_COLLECTION
        0x91, 0x02,                    // OUTPUT (Cnst,Ary,Abs)
          //  no padding ( 64 )
        0xc0                           // END_COLLECTION
        */
};

//bool hasKeyPressed = false;


/**
 * Callback invoked when received Set LED from central.
 * Must be set previously with setKeyboardLedCallback()
 *
 * The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
 *    Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
 */
//void set_keyboard_led(uint16_t conn_handle, uint8_t led_bitmap)
//{
//    (void) conn_handle;
//
//    // light up Red Led if any bits is set
//    if ( led_bitmap )
//    {
//        ledOn( LED_RED );
//    }
//    else
//    {
//        ledOff( LED_RED );
//    }
//}


void startAdv(void)
{
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_JOYSTICK);

    // Include BLE HID service
//    Bluefruit.Advertising.addService(blehid);
    Bluefruit.Advertising.addService(blebat);

    // There is enough room for the dev name in the advertising packet
    Bluefruit.Advertising.addName();

    /* Start Advertising
     * - Enable auto advertising if disconnected
     * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     * - Timeout for fast mode is 30 seconds
     * - Start(timeout) with timeout = 0 will advertise forever (until connected)
     *
     * For recommended advertising interval
     * https://developer.apple.com/library/content/qa/qa1931/_index.html
     */
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void cccd_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value)
{
    // Display the raw request packet
    Serial.print("CCCD Updated: ");
    //Serial.printBuffer(request->data, request->len);
    Serial.print(cccd_value);
    Serial.println("");

    // Check the characteristic this CCCD update is associated with in case
    // this handler is used for multiple CCCD records.
    if (chr->uuid == blebatc.uuid) {
        if (chr->indicateEnabled(conn_hdl)) {
            Serial.println("Temperature Measurement 'Indicate' enabled");
        } else {
            Serial.println("Temperature Measurement 'Indicate' disabled");
        }
    }
}

void setup_battery()
{
    blebat.begin();
    blebatc.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
    blebatc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    blebatc.setFixedLen(1);
    blebatc.setCccdWriteCallback(cccd_callback); //Not needed to enable notify feature, it's done by setProperties. However, service will not be implemented correctly.
    blebatc.begin();

    blebatc.write8(42);
}

void setup()
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

    Bluefruit.begin();
    Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
    Bluefruit.setName(BLEDIS_NAME);

    // Configure and Start Device Information Service
    bledis.setManufacturer(BLEDIS_MANUFACTURER);
    bledis.setModel(BLEDIS_MODEL);
    bledis.begin();

    /* Start BLE HID
     * Note: Apple requires BLE device must have min connection interval >= 20m
     * ( The smaller the connection interval the faster we could send data).
     * However for HID and MIDI device, Apple could accept min connection interval
     * up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
     * connection interval to 11.25  ms and 15 ms respectively for best performance.
     */
//    blehid.begin();

    // Set callback for set LED from central
//    blehid.setKeyboardLedCallback(set_keyboard_led);

    /* Set connection interval (min, max) to your perferred value.
     * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
     * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms
     */
    /* Bluefruit.Periph.setConnInterval(9, 12); */

    // Set up and start advertising
    setup_battery();

    hidService.setReportMap(joystick_report_desc, sizeof joystick_report_desc);
    hidService.begin();

    startAdv();
}

int loops = 0;
int battery = 0;

void loop()
{
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
    delay(5);
    loops++;
    if(loops > 1000){
        loops = 0;
        battery = (battery + 1) % 1000;
        blebatc.notify8(battery);
        Serial.println("Set battery");

    }
}
