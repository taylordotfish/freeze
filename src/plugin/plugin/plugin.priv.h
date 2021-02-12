static void freeze_plugin_reset_changed_flags(FreezePlugin *self);

static void freeze_plugin_reset_state(FreezePlugin *self);

static void freeze_plugin_run_playing(
    FreezePlugin *self,
    StereoSlice input,
    StereoPort output
);

static void maybe_update_ui(FreezePlugin *self);

static void forward_samples(StereoSlice input, StereoPort output);

static void on_client_get(void *context, UNUSED void *unused);

static void on_client_mode(void *context, FreezeRecordingMode mode);

static void on_client_clear_db(void *context, UNUSED void *unused);
