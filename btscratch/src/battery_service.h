//
// Created by chris on 2021-05-03.
//

#ifndef BTSCRATCH_BATTERY_SERVICE_H
#define BTSCRATCH_BATTERY_SERVICE_H

#include "BLEService.h"
#include "BLECharacteristic.h"
#include "BLEUuid.h"

class BLEBatteryService : public BLEService
{
public:
    BLEBatteryService();
    err_t begin() override;

private:
    BLECharacteristic* battery_characteristic;
    static void cccdCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value);
};



#endif //BTSCRATCH_BATTERY_SERVICE_H
