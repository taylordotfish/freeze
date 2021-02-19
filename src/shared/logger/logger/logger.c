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

// @guard FREEZE__SHARED__LOGGER__LOGGER_H

#include "logger.h"
#include "logger.priv.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#ifdef HEADER
    #include <stdarg.h>
    #include <stdbool.h>

    typedef enum PluginLogLevel {
        PLUGIN_LOG_TRACE = 0,
        PLUGIN_LOG_NOTE = 1,
        PLUGIN_LOG_WARN = 2,
        PLUGIN_LOG_ERROR = 3,
    } PluginLogLevel;

    typedef struct PluginLogger PluginLogger;

    typedef void (*PluginLogFunction)(
        const PluginLogger *self,
        PluginLogLevel level,
        const char *fmt,
        va_list args
    );

    struct PluginLogger {
        PluginLogFunction log;
        void *data;
        const char *name;
        bool debug;
    };
#endif

void plugin_log_trace(const PluginLogger *logger, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger->log(logger, PLUGIN_LOG_TRACE, fmt, args);
    va_end(args);
}

void plugin_log_note(const PluginLogger *logger, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger->log(logger, PLUGIN_LOG_NOTE, fmt, args);
    va_end(args);
}

void plugin_log_warn(const PluginLogger *logger, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger->log(logger, PLUGIN_LOG_WARN, fmt, args);
    va_end(args);
}

void plugin_log_error(const PluginLogger *logger, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger->log(logger, PLUGIN_LOG_ERROR, fmt, args);
    va_end(args);
}

static const char *level_to_str(PluginLogLevel level) {
    switch (level) {
        case PLUGIN_LOG_TRACE:
            return "trace";
        case PLUGIN_LOG_NOTE:
            return "note";
        case PLUGIN_LOG_WARN:
            return "warn";
        case PLUGIN_LOG_ERROR:
            return "error";
        default:
            break;
    }
    return "?";
}

static void fallback_log_func(
    const PluginLogger *logger,
    PluginLogLevel level,
    const char *fmt,
    va_list args
) {
    assert(logger != NULL);
    if (!logger->debug && level == PLUGIN_LOG_TRACE) {
        return;
    }
    fprintf(stderr, "[%s] [%s] ", logger->name, level_to_str(level));
    vfprintf(stderr, fmt, args);
    fputs("\n", stderr);
}

PluginLogger plugin_logger_fallback = {
    .log = fallback_log_func,
    .data = NULL,
    .name = "?",
    .debug = false,
};
