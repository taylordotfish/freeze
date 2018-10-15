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

// @guard GUI__MODELS__APP_H

#include "app.h"
#include "utils/string/string.h"

#ifdef HEADER
    #include "shared/mode/mode.h"
    #include <stdbool.h>
    #include <stddef.h>

    typedef struct AppModel {
        char *path;
        FreezeRecordingMode recording_mode;
        size_t memory_used;
    } AppModel;
#endif

void app_model_init(AppModel *self) {
    self->path = NULL;
    self->recording_mode = FREEZE_MODE_PLAYING;
    self->memory_used = 0;
}

const char *app_model_get_path(const AppModel *self) {
    return self->path;
}

void app_model_set_path(AppModel *self, const char *path) {
    free(self->path);
    self->path = strdup_or_abort(path);
}

FreezeRecordingMode app_model_get_recording_mode(const AppModel *self) {
    return self->recording_mode;
}

void app_model_set_recording_mode(AppModel *self, FreezeRecordingMode mode) {
    self->recording_mode = mode;
}

size_t app_model_get_memory_used(const AppModel *self) {
    return self->memory_used;
}

void app_model_set_memory_used(AppModel *self, size_t memory_used) {
    self->memory_used = memory_used;
}

void app_model_destroy(AppModel *self) {
    free(self->path);
}
