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

// @guard PLUGIN__LV2_H

#include "lv2.h"
#include "lv2.priv.h"
#include "utils/memory/memory.h"
#include "shared/lv2_util/lv2_util.h"
#include "shared/ports/ports.h"

#include <inttypes.h>
#include <string.h>

#ifdef HEADER
    #include "../plugin/plugin.h"
    #include "shared/logger/logger.h"

    typedef struct FreezeLV2 {
        FreezePlugin plugin;
        FreezeClient client;
        PluginLogger logger;
        LV2_Log_Logger lv2_logger;
        FreezeURIs uris;

        LV2_Atom_Sequence *notify_port;
        LV2_Atom_Forge notify_forge;
        LV2_Atom_Forge_Frame notify_frame;

        LV2_Atom_Sequence *control_port;
        float *in_left;
        float *in_right;
        float *out_left;
        float *out_right;
    } FreezeLV2;
#endif

#ifdef PRIVATE_HEADER
    #include <lv2/lv2plug.in/ns/ext/state/state.h>
#endif

#define FREEZE_DB_NAME "FreezeDB"

#pragma GCC diagnostic ignored "-Wunused-parameter"

static void write_atom(void *context, const LV2_Atom *atom) {
    FreezeLV2 *self = context;
    if (self->notify_port == NULL) {
        plugin_log_warn(
            &self->logger,
            "Plugin sent atom event, but the notify port isn't connected."
        );
        return;
    }
    lv2_atom_forge_frame_time(&self->notify_forge, 0);
    lv2_atom_forge_write(&self->notify_forge, atom, lv2_atom_total_size(atom));
}

static LV2_Handle instantiate(
        const LV2_Descriptor *descriptor, double sample_rate,
        const char *bundle_path, const LV2_Feature * const *features) {
    FreezeLV2 *self = malloc_or_abort(sizeof(FreezeLV2));
    plugin_logger_fallback.name = "Freeze/fallback";
    self->logger = plugin_logger_fallback;
    self->notify_port = NULL;
    self->notify_frame = (LV2_Atom_Forge_Frame){ 0 };

    LV2_Log_Log *log = NULL;
    LV2_URID_Map *map = NULL;
    const char *missing = lv2_features_query(
        features,
        LV2_LOG__log, &log, false,
        LV2_URID__map, &map, true,
        NULL
    );

    if (log == NULL) {
        plugin_log_warn(&self->logger, "LV2 logger not supplied.");
    } else {
        lv2_log_logger_init(&self->lv2_logger, map, log);
        self->logger = plugin_logger_from_lv2(&self->lv2_logger, "Freeze");
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

    lv2_atom_forge_init(&self->notify_forge, map);
    freeze_uris_init(&self->uris, map);
    freeze_client_init(&self->client, &self->uris, write_atom, self);
    freeze_client_set_logger(&self->client, &self->logger);
    freeze_plugin_init(&self->plugin, &self->client);
    freeze_plugin_set_logger(&self->plugin, &self->logger);
    plugin_log_trace(&self->logger, "Plugin instantiated.");
    return (LV2_Handle)self;
}

static void activate(LV2_Handle instance) {
    FreezeLV2 *self = (FreezeLV2 *)instance;
    freeze_plugin_activate(&self->plugin);
}

static void deactivate(LV2_Handle instance) {
    FreezeLV2 *self = (FreezeLV2 *)instance;
    freeze_plugin_deactivate(&self->plugin);
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data) {
    FreezeLV2 *self = (FreezeLV2 *)instance;
    switch (port) {
        case FREEZE_LV2_PORT_IN_L:
            self->in_left = data;
            break;

        case FREEZE_LV2_PORT_IN_R:
            self->in_right = data;
            break;

        case FREEZE_LV2_PORT_OUT_L:
            self->out_left = data;
            break;

        case FREEZE_LV2_PORT_OUT_R:
            self->out_right = data;
            break;

        case FREEZE_LV2_PORT_CONTROL:
            self->control_port = data;
            break;

        case FREEZE_LV2_PORT_NOTIFY:
            self->notify_port = data;
            break;

        default:
            plugin_log_warn(
                &self->logger,
                "Received request to connect unknown port %"PRIu32, port
            );
            break;
    }
}

static bool check_atom(
        FreezeLV2 *self, LV2_Atom *atom, LV2_URID expected_urid,
        const char *name) {
    if (atom == NULL) {
        plugin_log_error(
            &self->logger, "Expected to receive \"%s\" atom.", name
        );
        return false;
    }
    if (atom->type != expected_urid) {
        plugin_log_error(
            &self->logger, "\"%s\" atom is incorrect type.", name
        );
        return false;
    }
    return true;
}

static void update_position(FreezeLV2 *self, const LV2_Atom_Object *obj) {
    LV2_Atom *frame_atom = NULL;
    LV2_Atom *speed_atom = NULL;
    lv2_atom_object_get(
        obj,
        self->uris.time_frame, &frame_atom,
        self->uris.time_speed, &speed_atom,
        NULL
    );

    bool valid = (
        check_atom(self, frame_atom, self->uris.atom_Long, "time:frame") &&
        check_atom(self, speed_atom, self->uris.atom_Float, "time:speed")
    );
    if (!valid) {
        return;
    }

    uint_least32_t frame = ((const LV2_Atom_Long *)frame_atom)->body;
    freeze_plugin_set_frame(&self->plugin, frame);
    float speed = ((const LV2_Atom_Float *)speed_atom)->body;
    freeze_plugin_set_speed(&self->plugin, speed);
}

static void run(LV2_Handle instance, uint32_t sample_count) {
    FreezeLV2 *self = (FreezeLV2 *)instance;
    uint_least32_t notify_capacity = self->notify_port->atom.size;
    lv2_atom_forge_set_buffer(
        &self->notify_forge, (uint8_t *)self->notify_port, notify_capacity
    );
    lv2_atom_forge_sequence_head(&self->notify_forge, &self->notify_frame, 0);

    StereoSlice input = {
        .left = self->in_left,
        .right = self->in_right,
        .length = 0,
    };
    StereoPort output = {
        .left = self->out_left,
        .right = self->out_right,
    };

    uint_least32_t current_frame = 0;
    LV2_ATOM_SEQUENCE_FOREACH(self->control_port, event) {
        input.length = event->time.frames - current_frame;
        current_frame = event->time.frames;
        freeze_plugin_run(&self->plugin, input, output);

        input.left += input.length;
        input.right += input.length;
        output.left += input.length;
        output.right += input.length;

        uint_least32_t event_type = event->body.type;
        if (!lv2_atom_forge_is_object_type(&self->notify_forge, event_type)) {
            plugin_log_warn(
                &self->logger, "Control port received non-object atom."
            );
            continue;
        }

        const LV2_Atom_Object *obj = (const LV2_Atom_Object *)&event->body;
        if (obj->body.otype == self->uris.time_Position) {
            update_position(self, obj);
            continue;
        }

        freeze_client_on_event(&self->client, &event->body);
    }
    input.length = sample_count - current_frame;
    freeze_plugin_run(&self->plugin, input, output);
}

static void cleanup(LV2_Handle instance) {
    FreezeLV2 *self = (FreezeLV2 *)instance;
    freeze_plugin_destroy(&self->plugin);
    freeze_client_destroy(&self->client);
    free(self);
}

static LV2_State_Status save(
        LV2_Handle instance, LV2_State_Store_Function store,
        LV2_State_Handle handle, uint32_t flags,
        const LV2_Feature * const *features) {
    FreezeLV2 *self = (FreezeLV2 *)instance;
    LV2_State_Map_Path *map_path = NULL;
    LV2_State_Make_Path *make_path = NULL;
    const char *missing = lv2_features_query(
        features,
        LV2_STATE__mapPath, &map_path, true,
        LV2_STATE__makePath, &make_path, true,
        NULL
    );

    if (missing != NULL) {
        plugin_log_error(
            &self->logger, "Missing required feature <%s>", missing
        );
        return LV2_STATE_ERR_NO_FEATURE;
    }

    if (freeze_plugin_get_db_path(&self->plugin) == NULL) {
        char *db_path = make_path->path(make_path->handle, FREEZE_DB_NAME);
        freeze_plugin_set_db_path(&self->plugin, db_path);
        free(db_path);
    }

    char *abst_db_path = map_path->abstract_path(
        map_path->handle, freeze_plugin_get_db_path(&self->plugin)
    );
    store(
        handle, self->uris.freeze_db_path, abst_db_path,
        strlen(abst_db_path) + 1, self->uris.atom_Path,
        LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE
    );
    free(abst_db_path);

    plugin_log_trace(&self->logger, "Saving database...");
    freeze_plugin_save_db(&self->plugin);
    plugin_log_trace(&self->logger, "Done saving database.");
    return LV2_STATE_SUCCESS;
}

static LV2_State_Status restore(
        LV2_Handle instance, LV2_State_Retrieve_Function retrieve,
        LV2_State_Handle handle, uint32_t flags,
        const LV2_Feature * const *features) {
    FreezeLV2 *self = (FreezeLV2 *)instance;
    LV2_State_Map_Path *map_path = NULL;
    const char *missing = lv2_features_query(
        features,
        LV2_STATE__mapPath, &map_path, true,
        NULL
    );

    if (missing != NULL) {
        plugin_log_error(
            &self->logger, "Missing required feature <%s>", missing
        );
        return LV2_STATE_ERR_NO_FEATURE;
    }

    size_t size = 0;
    uint32_t type = 0;
    uint32_t val_flags = 0;
    const char *abst_db_path = retrieve(
        handle, self->uris.freeze_db_path, &size, &type, &val_flags
    );

    if (abst_db_path == NULL) {
        plugin_log_trace(
            &self->logger,
            "Missing freeze:db-path in state; assuming new plugin instance."
        );
        return LV2_STATE_SUCCESS;
    }

    char *db_path = map_path->absolute_path(map_path->handle, abst_db_path);
    freeze_plugin_set_db_path(&self->plugin, db_path);
    free(db_path);

    plugin_log_trace(&self->logger, "Loading database...");
    freeze_plugin_load_db(&self->plugin);
    plugin_log_trace(&self->logger, "Done loading database.");
    return LV2_STATE_SUCCESS;
}

static const void *extension_data(const char *uri) {
    static const LV2_State_Interface state = { save, restore };
    if (strcmp(uri, LV2_STATE__interface) == 0) {
        return &state;
    }
    return NULL;
}

static const LV2_Descriptor descriptor = {
    .URI = FREEZE_URI,
    .instantiate = instantiate,
    .activate = activate,
    .deactivate = deactivate,
    .cleanup = cleanup,
    .connect_port = connect_port,
    .run = run,
    .extension_data = extension_data,
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor *lv2_descriptor(uint32_t index) {
    switch (index) {
        case 0:
            return &descriptor;
        default:
            return NULL;
    }
}
