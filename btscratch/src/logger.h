//
// Created by chris on 2022-01-07.
//

#ifndef BTSCRATCH_LOGGER_H
#define BTSCRATCH_LOGGER_H

#include <Arduino.h>
#include "NullLogger.h"

static NullLogger nullLogger = NullLogger();

extern Print* logger;

#endif //BTSCRATCH_LOGGER_H
