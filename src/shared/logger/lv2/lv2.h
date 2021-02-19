#ifndef FREEZE__SHARED__LOGGER__LV2_H
#define FREEZE__SHARED__LOGGER__LV2_H

#include "../logger/logger.h"
#include <lv2/lv2plug.in/ns/ext/log/logger.h>

PluginLogger plugin_logger_from_lv2(
    LV2_Log_Logger *lv2_logger,
    const char *name
);

#endif
