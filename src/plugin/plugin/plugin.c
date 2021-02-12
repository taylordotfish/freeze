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

// @guard FREEZE__PLUGIN__PLUGIN_H

#include "plugin.h"
#include "utils/string/string.h"
#include "utils/unused/unused.h" /* @include */
#include "plugin.priv.h"
#include <math.h>

#ifdef HEADER
    #include "../recording/recording.h"
    #include "../samples/samples.h"
    #include "shared/client/client.h"
    #include "shared/logger/logger.h"
    #include "shared/mode/mode.h"
    #include <stdbool.h>

    typedef struct FreezePlugin {
        Recording recording;
        uint_least32_t frame;
        FreezeRecordingMode mode;

        FreezeClient *client;
        const PluginLogger *logger;
        size_t samples_since_ui_msg;

        char *db_path;
        bool db_path_changed: 1;
        bool ui_initialized: 1;
        bool active: 1;
        bool playing: 1;
    } FreezePlugin;
#endif

#define SPEED_EPSILON 1e-6

// How many samples must pass before `freeze_plugin_run` sends UI updates.
#define SAMPLES_PER_UI_UPDATE 24000

void freeze_plugin_init(FreezePlugin *self, FreezeClient *client) {
    recording_init(&self->recording);
    self->db_path = NULL;
    freeze_plugin_reset_changed_flags(self);

    self->client = client;
    self->logger = &plugin_logger_fallback;
    self->samples_since_ui_msg = SAMPLES_PER_UI_UPDATE;
    self->ui_initialized = false;

    FREEZE_CLIENT_ADD_CALLBACK(client, ON_GET, on_client_get, self);
    FREEZE_CLIENT_ADD_CALLBACK(client, ON_MODE, on_client_mode, self);
    FREEZE_CLIENT_ADD_CALLBACK(client, ON_CLEAR_DB, on_client_clear_db, self);
    freeze_plugin_reset_state(self);
}

static void freeze_plugin_reset_changed_flags(FreezePlugin *self) {
    self->db_path_changed = false;
}

const char *freeze_plugin_get_db_path(const FreezePlugin *self) {
    return self->db_path;
}

const char *freeze_plugin_get_db_path_or_empty(const FreezePlugin *self) {
    return self->db_path == NULL ? "" : self->db_path;
}

void freeze_plugin_set_db_path(FreezePlugin *self, const char *path) {
    free(self->db_path);
    self->db_path = strdup_or_abort(path);
    self->db_path_changed = true;
}

void freeze_plugin_load_db(FreezePlugin *self) {
    if (string_null_or_empty(self->db_path)) {
        plugin_log_warn(self->logger, "Tried to load from empty DB path.");
        return;
    }
    recording_load_db(&self->recording, self->db_path);
}

void freeze_plugin_save_db(FreezePlugin *self) {
    if (string_null_or_empty(self->db_path)) {
        plugin_log_warn(self->logger, "Tried to save to empty DB path.");
        return;
    }
    recording_save_db(&self->recording, self->db_path);
}

static void freeze_plugin_reset_state(FreezePlugin *self) {
    self->frame = 0;
    self->mode = FREEZE_MODE_PLAYING;
    self->playing = false;
}

void freeze_plugin_activate(FreezePlugin *self) {
    self->active = true;
    freeze_plugin_reset_state(self);
    plugin_log_trace(self->logger, "Plugin activated.");
}

void freeze_plugin_deactivate(FreezePlugin *self) {
    self->active = false;
    plugin_log_trace(self->logger, "Plugin deactivated.");
}

void freeze_plugin_set_frame(FreezePlugin *self, uint_least32_t frame) {
    self->frame = frame;
}

void freeze_plugin_set_speed(FreezePlugin *self, float speed) {
    self->playing = fabs(speed) > SPEED_EPSILON;
}

void freeze_plugin_run(
    FreezePlugin *self,
    StereoSlice input,
    StereoPort output
) {
    if (self->playing) {
        freeze_plugin_run_playing(self, input, output);
    } else {
        memset(output.left, 0, input.length * sizeof(float));
        memset(output.right, 0, input.length * sizeof(float));
    }

    self->samples_since_ui_msg += input.length;
    maybe_update_ui(self);
}

static void freeze_plugin_run_playing(
    FreezePlugin *self,
    StereoSlice input,
    StereoPort output
) {
    switch (self->mode) {
        case FREEZE_MODE_PLAYING:
            recording_get(&self->recording, self->frame, input.length, output);
            break;

        case FREEZE_MODE_RECORDING:
            recording_set(&self->recording, self->frame, input);
            forward_samples(input, output);
            break;

        default:
            break;
    }
    self->frame += input.length;
}

static void maybe_update_ui(FreezePlugin *self) {
    if (!self->ui_initialized) {
        return;
    }
    if (self->samples_since_ui_msg < SAMPLES_PER_UI_UPDATE) {
        return;
    }
    bool any_update = false;
    if (any_update |= self->db_path_changed) {
        freeze_client_set_db_path(
            self->client, freeze_plugin_get_db_path_or_empty(self)
        );
    }
    if (any_update |= self->mode == FREEZE_MODE_RECORDING) {
        freeze_client_set_mem_used(
            self->client, recording_get_memory_used(&self->recording)
        );
    }
    if (any_update) {
        self->samples_since_ui_msg = 0;
        freeze_plugin_reset_changed_flags(self);
    }
}

static void forward_samples(StereoSlice input, StereoPort output) {
    memcpy(output.left, input.left, input.length * sizeof(*input.left));
    memcpy(output.right, input.right, input.length * sizeof(*input.right));
}

void freeze_plugin_set_logger(FreezePlugin *self, const PluginLogger *logger) {
    self->logger = logger;
    recording_set_logger(&self->recording, logger);
}

void freeze_plugin_destroy(FreezePlugin *self) {
    recording_destroy(&self->recording);
    free(self->db_path);
}



/**************************/
/* FreezeClient callbacks */
/**************************/

static void on_client_get(void *context, UNUSED void *unused) {
    FreezePlugin *self = context;
    freeze_client_set_mode(self->client, self->mode);
    freeze_client_set_db_path(
        self->client, freeze_plugin_get_db_path_or_empty(self)
    );
    freeze_client_set_mem_used(
        self->client, recording_get_memory_used(&self->recording)
    );
    self->samples_since_ui_msg = 0;
    self->ui_initialized = true;
    freeze_plugin_reset_changed_flags(self);
}

static void on_client_mode(void *context, FreezeRecordingMode mode) {
    FreezePlugin *self = context;
    plugin_log_trace(
        self->logger, "Received request to change mode to %d.", mode
    );
    self->mode = mode;
    freeze_client_set_mode(self->client, self->mode);
    freeze_client_set_mem_used(
        self->client, recording_get_memory_used(&self->recording)
    );
    self->samples_since_ui_msg = 0;
}

static void on_client_clear_db(void *context, UNUSED void *unused) {
    FreezePlugin *self = context;
    plugin_log_trace(self->logger, "Received request to clear DB.");
    recording_clear(&self->recording);
    freeze_client_set_mem_used(
        self->client, recording_get_memory_used(&self->recording)
    );
    self->samples_since_ui_msg = 0;
}
