//
// Created by chris on 2021-05-03.
//

#include "battery_service.h"

#include "BLEService.h"
#include "BLECharacteristic.h"
#include "BLEUuid.h"
#include <Arduino.h>
#include <bluefruit.h>



BLEBatteryService::BLEBatteryService() : BLEService(UUID16_SVC_BATTERY) {

};

err_t BLEBatteryService::begin() {
    BLEService::begin();

    battery_characteristic = new BLECharacteristic(UUID16_CHR_BATTERY_LEVEL);

    battery_characteristic->setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
    battery_characteristic->setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    battery_characteristic->setFixedLen(1);
    battery_characteristic->setCccdWriteCallback(BLEBatteryService::cccdCallback); //Not needed to enable notify feature, it's done by setProperties. However, service will not be implemented correctly.
    battery_characteristic->begin();
    battery_characteristic->write8(100);
    battery_characteristic->notify8(100);

    return 0;
};

void BLEBatteryService::cccdCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value){
    Serial.print("CCCD Updated: ");
    Serial.printBuffer(request->data, request->len);
    Serial.print(cccd_value);
    Serial.println("");

    if(chr->uuid == UUID16_SVC_BATTERY) {
        if (chr->indicateEnabled(conn_hdl)) {
            Serial.println("Battery service 'Indicate' enabled");
        } else {
            Serial.println("Battery service 'Indicate' disabled");
        }
    }
};