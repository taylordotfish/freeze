static const char *level_to_str(PluginLogLevel level);

static void fallback_log_func(
    const PluginLogger *logger,
    PluginLogLevel level,
    const char *fmt,
    va_list args
);
