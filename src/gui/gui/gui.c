/*
 * Copyright (C) 2021 taylor.fish <contact@taylor.fish>
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

// @guard FREEZE__GUI__GUI_H

#include "gui.h"
#include "gui.priv.h"
#include "../presentation/presentation.h"

#ifdef HEADER
    #include "../models/models.h"
    #include "../views/views.h"
    #include <shared/client/client.h>

    typedef struct FreezeGUI {
        AppModel model;
        AppView view;
        FreezeClient *client;
    } FreezeGUI;
#endif

void freeze_gui_init(FreezeGUI *self, FreezeClient *client) {
    app_model_init(&self->model);
    app_view_init(&self->view);
    self->client = client;

    FREEZE_CLIENT_ADD_CALLBACK(client, ON_MODE, on_client_mode, self);
    FREEZE_CLIENT_ADD_CALLBACK(client, ON_PATH, on_client_path, self);
    FREEZE_CLIENT_ADD_CALLBACK(client, ON_MEM_USED, on_client_mem_used, self);

    // Request plugin state.
    freeze_client_request_state(client);
    freeze_client_write(client);
}

GtkWidget *freeze_gui_widget(const FreezeGUI *self) {
    return app_view_widget(&self->view);
}

void freeze_gui_destroy(FreezeGUI *self) {
    app_model_destroy(&self->model);
}

static void freeze_gui_update(FreezeGUI *self) {
    AppPres pres = {
        .model = &self->model,
        .client = self->client,
    };
    app_view_update(&self->view, &pres);
    app_model_reset_changed_flags(&self->model);
}



/**************************/
/* FreezeClient callbacks */
/**************************/

static void on_client_mode(void *context, FreezeRecordingMode mode) {
    FreezeGUI *self = context;
    app_model_set_recording_mode(&self->model, mode);
    freeze_gui_update(self);
}

static void on_client_path(void *context, const char *path) {
    FreezeGUI *self = context;
    app_model_set_path(&self->model, path);
    freeze_gui_update(self);
}

static void on_client_mem_used(void *context, size_t mem_used) {
    FreezeGUI *self = context;
    app_model_set_memory_used(&self->model, mem_used);
    freeze_gui_update(self);
}
