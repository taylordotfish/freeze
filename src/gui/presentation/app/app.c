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

// @guard FREEZE__GUI__PRESENTATION__APP_H

#include "app.h"
#include "utils/utils.h"
#include <assert.h>
#include <stdio.h>

#ifdef HEADER
    #include "../record_buttons/record_buttons.h"
    #include "../../models/app/app.h"
    #include "shared/client/client.h"
    #include <stdbool.h>

    typedef struct AppPres {
        AppModel *model;
        FreezeClient *client;
    } AppPres;
#endif

char *app_pres_make_memory_text(const AppPres *self) {
    size_t memory_used = app_model_get_memory_used(self->model);
    return sprintf_heap(
        "Memory Used: %.1f MB", (double)memory_used / 1000 / 1000
    );
}

bool app_pres_memory_text_changed(const AppPres *self) {
    return self->model->memory_used_changed;
}

const char *app_pres_get_db_path(const AppPres *self) {
    return app_model_get_path(self->model);
}

bool app_pres_db_path_changed(const AppPres *self) {
    return self->model->path_changed;
}

RecordButtonsPres app_pres_get_record_buttons(const AppPres *self) {
    return (RecordButtonsPres){
        .mode = app_model_get_recording_mode(self->model),
        .client = self->client,
    };
}
