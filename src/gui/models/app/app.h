#ifndef GUI__MODELS__APP_H
#define GUI__MODELS__APP_H

#include "shared/mode/mode.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct AppModel {
    char *path;
    FreezeRecordingMode recording_mode;
    size_t memory_used;
} AppModel;

void app_model_init(AppModel *self);

const char *app_model_get_path(const AppModel *self);

void app_model_set_path(AppModel *self, const char *path);

FreezeRecordingMode app_model_get_recording_mode(const AppModel *self);

void app_model_set_recording_mode(AppModel *self, FreezeRecordingMode mode);

size_t app_model_get_memory_used(const AppModel *self);

void app_model_set_memory_used(AppModel *self, size_t memory_used);

void app_model_free(AppModel *self);

#endif
