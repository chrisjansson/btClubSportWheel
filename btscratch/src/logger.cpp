#include "logger.h"

NullLogger nullLogger = NullLogger();
Print* logger = &nullLogger;