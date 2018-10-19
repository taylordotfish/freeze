/*
 * Copyright (C) 2018 taylor.fish <contact@taylor.fish>
 *
 * This file is part of Freeze.
 *
 * Freeze is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Freeze is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Freeze.  If not, see <http://www.gnu.org/licenses/>.
 */

// @guard FREEZE__SHARED__LOGGER__LV2_H

#include "lv2.h"
#include "lv2.priv.h"
#include "utils/string/string.h"

#ifdef HEADER
    #include "../logger/logger.h"
    #include <lv2/lv2plug.in/ns/ext/log/logger.h>
#endif

static LV2_URID level_to_urid(
        LV2_Log_Logger *lv2_logger, PluginLogLevel level) {
    switch (level) {
        case PLUGIN_LOG_TRACE:
            return lv2_logger->Trace;
        case PLUGIN_LOG_NOTE:
            return lv2_logger->Note;
        case PLUGIN_LOG_WARN:
            return lv2_logger->Warning;
        case PLUGIN_LOG_ERROR:
            return lv2_logger->Error;
        default:
            break;
    }
    return lv2_logger->Error;
}

static void lv2_log_printf(
        LV2_Log_Logger *logger, LV2_URID type, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    lv2_log_vprintf(logger, type, fmt, args);
    va_end(args);
}

static void lv2_log_func(
        const PluginLogger *logger, PluginLogLevel level, const char *fmt,
        va_list args) {
    if (level == PLUGIN_LOG_TRACE) {
        if (!logger->debug) {
            return;
        }
        level = PLUGIN_LOG_NOTE;
    }

    LV2_Log_Logger *lv2_logger = logger->data;
    char *msg = vsprintf_heap(fmt, args);
    lv2_log_printf(
        lv2_logger, level_to_urid(lv2_logger, level), "[%s] %s\n",
        logger->name, msg
    );
    free(msg);
}

PluginLogger plugin_logger_from_lv2(
        LV2_Log_Logger *lv2_logger, const char *name) {
    return (PluginLogger){
        .log = lv2_log_func,
        .data = lv2_logger,
        .name = name,
        .debug = false,
    };
}
