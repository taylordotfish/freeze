/*
 * Copyright (C) 2018, 2021 taylor.fish <contact@taylor.fish>
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

// @guard FREEZE__GUI__MODELS__APP_H

#include "app.h"
#include "app.priv.h"
#include <utils/string/string.h>

#ifdef HEADER
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
        bool path_changed;
        bool memory_used_changed;
    } AppModel;
#endif

void app_model_init(AppModel *self) {
    self->path = NULL;
    self->recording_mode = FREEZE_MODE_PLAYING;
    self->memory_used = 0;
    app_model_set_changed_flags(self, true);
}

void app_model_reset_changed_flags(AppModel *self) {
    app_model_set_changed_flags(self, false);
}

static void app_model_set_changed_flags(AppModel *self, bool value) {
    self->path_changed = value;
    self->memory_used_changed = value;
}

const char *app_model_get_path(const AppModel *self) {
    return self->path;
}

void app_model_set_path(AppModel *self, const char *path) {
    free(self->path);
    self->path = strdup_or_abort(path);
    self->path_changed = true;
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
    self->memory_used_changed = (memory_used != self->memory_used);
    self->memory_used = memory_used;
}

void app_model_destroy(AppModel *self) {
    free(self->path);
}
