static void freeze_gui_update(FreezeGUI *self);

static void on_client_mode(void *context, FreezeRecordingMode mode);

static void on_client_path(void *context, const char *path);

static void on_client_mem_used(void *context, size_t mem_used);
