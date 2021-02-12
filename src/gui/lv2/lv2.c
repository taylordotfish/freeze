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

// @guard FREEZE__GUI__LV2_H

#include "lv2.h"
#include "shared/lv2_util/lv2_util.h"
#include "shared/ports/ports.h"
#include "utils/memory/memory.h"
#include "utils/unused/unused.h" /* @include */
#include "lv2.priv.h"

#include <assert.h>
#include <string.h>

#ifdef HEADER
    #include "../gui/gui.h"
    #include "../models/models.h"
    #include "../presentation/presentation.h"
    #include "../views/views.h"
    #include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

    typedef struct FreezeLV2UI {
        FreezeGUI gui;
        FreezeURIs uris;
        FreezeClient client;
        PluginLogger logger;

        LV2UI_Write_Function write;
        LV2UI_Controller controller;
        LV2_Log_Logger lv2_logger;
    } FreezeLV2UI;
#endif

static void write_atom(void *context, const LV2_Atom *atom) {
    FreezeLV2UI *self = context;
    self->write(
        self->controller, FREEZE_LV2_PORT_CONTROL, lv2_atom_total_size(atom),
        self->uris.atom_eventTransfer, atom
    );
}

static LV2UI_Handle instantiate(
    UNUSED const LV2UI_Descriptor *descriptor,
    UNUSED const char *plugin_uri,
    UNUSED const char *bundle_path,
    LV2UI_Write_Function write_function,
    LV2UI_Controller controller,
    LV2UI_Widget *widget,
    const LV2_Feature * const *features
) {
    FreezeLV2UI *self = malloc_or_abort(sizeof(FreezeLV2UI));
    plugin_logger_fallback.name = "FreezeUI/fallback";
    self->logger = plugin_logger_fallback;
    self->write = write_function;
    self->controller = controller;

    LV2_Log_Log *log = NULL;
    LV2_URID_Map *map = NULL;
    const char *missing = lv2_features_query(
        features,
        LV2_LOG__log, &log, false,
        LV2_URID__map, &map, true,
        NULL
    );

    if (log != NULL) {
        lv2_log_logger_init(&self->lv2_logger, map, log);
        self->logger = plugin_logger_from_lv2(&self->lv2_logger, "FreezeUI");
    }
    if (missing) {
        plugin_log_error(
            &self->logger, "Missing required feature <%s>", missing
        );
        free(self);
        return NULL;
    }

    #if FREEZE_DEBUG
        plugin_logger_fallback.debug = true;
        self->logger.debug = true;
    #endif

    freeze_uris_init(&self->uris, map);
    freeze_client_init(&self->client, &self->uris, write_atom, self);
    freeze_client_set_logger(&self->client, &self->logger);
    freeze_gui_init(&self->gui, &self->client);

    GtkWidget *app_widget = freeze_gui_widget(&self->gui);
    *widget = (LV2UI_Widget)app_widget;
    plugin_log_trace(&self->logger, "UI instantiated.");
    return (LV2UI_Handle)self;
}

static void port_event(
    LV2UI_Handle handle,
    UNUSED uint32_t port_index,
    UNUSED uint32_t buffer_size,
    uint32_t format,
    const void *buffer
) {
    FreezeLV2UI *self = (FreezeLV2UI *)handle;
    if (format != self->uris.atom_eventTransfer) {
        plugin_log_warn(&self->logger, "Unknown port event format.");
        return;
    }
    const LV2_Atom *atom = buffer;
    freeze_client_on_event(&self->client, atom);
}

static void cleanup(LV2UI_Handle handle) {
    FreezeLV2UI *self = (FreezeLV2UI *)handle;
    freeze_gui_destroy(&self->gui);
    freeze_client_destroy(&self->client);
    free(self);
}

static const LV2UI_Descriptor descriptor = {
    .URI = FREEZE_UI_URI,
    .instantiate = instantiate,
    .port_event = port_event,
    .cleanup = cleanup,
    .extension_data = NULL,
};

LV2_SYMBOL_EXPORT
const LV2UI_Descriptor *lv2ui_descriptor(uint32_t index) {
    switch (index) {
        case 0:
            return &descriptor;
        default:
            return NULL;
    }
}
