static LV2_URID level_to_urid(
    LV2_Log_Logger *lv2_logger, PluginLogLevel level);

static void lv2_log_printf(
    LV2_Log_Logger *logger, LV2_URID type, const char *fmt, ...);

static void lv2_log_func(
    const PluginLogger *logger, PluginLogLevel level, const char *fmt,
    va_list args);
