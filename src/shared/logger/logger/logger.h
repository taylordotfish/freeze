#ifndef FREEZE__SHARED__LOGGER__LOGGER_H
#define FREEZE__SHARED__LOGGER__LOGGER_H

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

void plugin_log_trace(const PluginLogger *logger, const char *fmt, ...);

void plugin_log_note(const PluginLogger *logger, const char *fmt, ...);

void plugin_log_warn(const PluginLogger *logger, const char *fmt, ...);

void plugin_log_error(const PluginLogger *logger, const char *fmt, ...);

extern PluginLogger plugin_logger_fallback;

#endif
