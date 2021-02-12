#ifndef FREEZE__GUI__MODELS__APP_H
#define FREEZE__GUI__MODELS__APP_H

#include <shared/mode/mode.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct AppModel {
    char *path;
    FreezeRecordingMode recording_mode;
    size_t memory_used;

    // Changes are tracked for performance optimizations -- when updating
    // the GUI, we can skip certain parts of the update if we know the
    // data hasn't changed.
    bool path_changed: 1;
    bool memory_used_changed: 1;
} AppModel;

void app_model_init(AppModel *self);

void app_model_reset_changed_flags(AppModel *self);

const char *app_model_get_path(const AppModel *self);

void app_model_set_path(AppModel *self, const char *path);

FreezeRecordingMode app_model_get_recording_mode(const AppModel *self);

void app_model_set_recording_mode(AppModel *self, FreezeRecordingMode mode);

size_t app_model_get_memory_used(const AppModel *self);

void app_model_set_memory_used(AppModel *self, size_t memory_used);

void app_model_destroy(AppModel *self);

#endif
