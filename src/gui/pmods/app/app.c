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

// @guard FREEZE__GUI__PMODS__APP_H

#include "app.h"
#include "app.priv.h"
#include "utils/utils.h"
#include <assert.h>
#include <stdio.h>

#ifdef HEADER
    #include "gui/models/models.h"
    #include "shared/client/client.h"
    #include "../db_path/db_path.h"
    #include "../record_buttons/record_buttons.h"

    typedef struct AppPM {
        AppModel *model;
        FreezeClient *client;
        DBPathPM db_path;
        RecordButtonsPM record_buttons;

        void *sync_context;
        void (*sync)(void *context);
    } AppPM;
#endif

void app_pm_init(AppPM *self, AppModel *model, FreezeClient *client) {
    self->model = model;
    self->client = client;
    self->sync_context = NULL;
    self->sync = NULL;

    FREEZE_CLIENT_ADD_CALLBACK(client, ON_MODE, on_client_mode, self);
    FREEZE_CLIENT_ADD_CALLBACK(client, ON_PATH, on_client_path, self);
    FREEZE_CLIENT_ADD_CALLBACK(client, ON_MEM_USED, on_client_mem_used, self);

    // DBPathPM
    db_path_pm_init(&self->db_path);

    // RecordButtonsPM
    record_buttons_pm_init(&self->record_buttons);
    record_buttons_pm_set_callbacks(
        &self->record_buttons, self, on_rb_mode_change, on_rb_clear
    );

    // Request plugin state.
    freeze_client_request_state(client);
}

char *app_pm_make_memory_text(const AppPM *self) {
    size_t memory_used = app_model_get_memory_used(self->model);
    return sprintf_heap(
        "Memory Used: %.1f MB", (double)memory_used / 1000 / 1000
    );
}

void app_pm_sync(AppPM *self) {
    if (self->sync != NULL) {
        self->sync(self->sync_context);
    }

    db_path_pm_set_path(&self->db_path, app_model_get_path(self->model));
    record_buttons_pm_set_mode(
        &self->record_buttons, app_model_get_recording_mode(self->model)
    );

    db_path_pm_sync(&self->db_path);
    record_buttons_pm_sync(&self->record_buttons);
}

void app_pm_set_sync(AppPM *self, void *context, void (*sync)(void *)) {
    self->sync_context = context;
    self->sync = sync;
}

void app_pm_destroy(AppPM *self) {
    db_path_pm_destroy(&self->db_path);
    record_buttons_pm_destroy(&self->record_buttons);
}



/*****************************/
/* RecordButtonsPM callbacks */
/*****************************/

static void on_rb_mode_change(void *context, FreezeRecordingMode mode) {
    AppPM *self = context;
    freeze_client_set_mode(self->client, mode);
}

static void on_rb_clear(void *context) {
    AppPM *self = context;
    freeze_client_clear_db(self->client);
}



/**************************/
/* FreezeClient callbacks */
/**************************/

static void on_client_mode(void *context, FreezeRecordingMode mode) {
    AppPM *self = context;
    app_model_set_recording_mode(self->model, mode);
    app_pm_sync(self);
}

static void on_client_path(void *context, const char *path) {
    AppPM *self = context;
    app_model_set_path(self->model, path);
    app_pm_sync(self);
}

static void on_client_mem_used(void *context, size_t mem_used) {
    AppPM *self = context;
    app_model_set_memory_used(self->model, mem_used);
    app_pm_sync(self);
}
