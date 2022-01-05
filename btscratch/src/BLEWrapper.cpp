//
// Created by chris on 2021-12-30.
//

#include "BLEWrapper.h"
#include <Arduino.h>
#include <bluefruit.h>
#include "manifest.h"
#include "joystick_report_descriptor.h"
#include "joystick_report_builder.h"
#include "battery_service.h"

void BLEWrapper::setup() {
    setupServices();
    startAdvertising();
}

BLEDis bledis;
BLEBatteryService blebat = BLEBatteryService();
extern BLEHidGeneric hidService;

void BLEWrapper::setupServices() {

    Bluefruit.begin();
    Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
    Bluefruit.setName(BLEDIS_NAME);

    // Configure and Start Device Information Service
    bledis.setManufacturer(BLEDIS_MANUFACTURER);
    bledis.setModel(BLEDIS_MODEL);
    bledis.begin();

    /* Start BLEWrapper HID
     * Note: Apple requires BLEWrapper device must have min connection interval >= 20m
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
    hidService.setReportMap(joystick_report_desc, sizeof joystick_report_desc);
    hidService.begin();

    // Set up and start advertising
    blebat.begin();
}

void BLEWrapper::startAdvertising() {
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_JOYSTICK);

    // Include BLEWrapper HID service
    Bluefruit.Advertising.addService(hidService);
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

void BLEWrapper::setConnectCallback(ble_connect_callback_t fp) {
    Bluefruit.Periph.setConnectCallback(fp);
}

void BLEWrapper::setDisconnectCallback(ble_disconnect_callback_t fp) {
    Bluefruit.Periph.setDisconnectCallback(fp);

}