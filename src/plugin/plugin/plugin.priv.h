static void freeze_plugin_reset_state(FreezePlugin *self);

static void forward_samples(StereoSlice input, StereoPort output);

static void on_client_get(void *context, void *unused);

static void on_client_mode(void *context, FreezeRecordingMode mode);

static void on_client_clear_db(void *context, void *unused);
