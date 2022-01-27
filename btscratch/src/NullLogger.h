//
// Created by chris on 2022-01-07.
//

#ifndef BTSCRATCH_NULLLOGGER_H
#define BTSCRATCH_NULLLOGGER_H

#include <Arduino.h>

class NullLogger : public Print {
    size_t write(uint8_t) override {
        return 0;
    }
};

#endif //BTSCRATCH_NULLLOGGER_H
