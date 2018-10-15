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

// @guard SHARED__CLIENT__CLIENT_H

#include "client.h"
#include "client.priv.h"
#include "utils/int/int.h"

#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HEADER
    #include "../callbacks/callbacks.h"
    #include "shared/logger/logger.h"
    #include "shared/uris/uris.h"

    #include <lv2/lv2plug.in/ns/ext/atom/forge.h>
    #include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
    #include <stdint.h>

    typedef void (*AtomWriteFunc)(void *context, const LV2_Atom *atom);

    typedef struct FreezeClient {
        FreezeClientCallbacks callbacks;

        const FreezeURIs *uris;
        AtomWriteFunc write;
        void *write_context;

        LV2_Atom_Forge forge;
        uint8_t forge_buffer[2048];
        const PluginLogger *logger;
    } FreezeClient;
#endif

#define CHECK_FORGE(ref) do { \
    if (ref == 0) { \
        fprintf(stderr, "At line %d in %s:\n", __LINE__, __FILE__); \
        fprintf(stderr, "Exceeded buffer space when forging LV2 atom.\n"); \
        abort(); \
    } \
} while (0)

void freeze_client_init(
        FreezeClient *self, const FreezeURIs *uris, AtomWriteFunc write,
        void *write_context) {
    self->uris = uris;
    self->write = write;
    self->write_context = write_context;
    self->logger = &plugin_logger_fallback;

    freeze_client_callbacks_init(&self->callbacks);
    lv2_atom_forge_init(&self->forge, uris->map);
}

static void freeze_client_reset_forge(FreezeClient *self) {
    lv2_atom_forge_set_buffer(
        &self->forge, self->forge_buffer, sizeof(self->forge_buffer)
    );
}

static void freeze_client_write(
        FreezeClient *self, const LV2_Atom_Forge_Ref ref) {
    self->write(self->write_context, (const LV2_Atom *)ref);
}



/*********************/
/* Sending functions */
/*********************/

void freeze_client_request_state(FreezeClient *self) {
    freeze_client_reset_forge(self);
    LV2_Atom_Forge *forge = &self->forge;
    const FreezeURIs *uris = self->uris;

    LV2_Atom_Forge_Frame frame;
    LV2_Atom_Forge_Ref atom = lv2_atom_forge_object(
        forge, &frame, 0, uris->patch_Get
    );
    CHECK_FORGE(atom);
    lv2_atom_forge_pop(forge, &frame);
    freeze_client_write(self, atom);
}

static LV2_Atom_Forge_Ref freeze_client_forge_patch_set(
        FreezeClient *self, LV2_Atom_Forge_Frame *frame) {
    freeze_client_reset_forge(self);
    LV2_Atom_Forge_Ref atom = lv2_atom_forge_object(
        &self->forge, frame, 0, self->uris->patch_Set
    );
    CHECK_FORGE(atom);
    return atom;
}

void freeze_client_set_mode(FreezeClient *self, FreezeRecordingMode mode) {
    LV2_Atom_Forge_Frame frame;
    LV2_Atom_Forge_Ref atom = freeze_client_forge_patch_set(self, &frame);
    LV2_Atom_Forge *forge = &self->forge;
    const FreezeURIs *uris = self->uris;

    CHECK_FORGE(lv2_atom_forge_key(forge, uris->patch_property));
    CHECK_FORGE(lv2_atom_forge_urid(forge, uris->freeze_recording_mode));
    CHECK_FORGE(lv2_atom_forge_key(forge, uris->patch_value));
    CHECK_FORGE(lv2_atom_forge_int(forge, mode));
    lv2_atom_forge_pop(forge, &frame);
    freeze_client_write(self, atom);
}

void freeze_client_set_db_path(FreezeClient *self, const char *db_path) {
    LV2_Atom_Forge_Frame frame;
    LV2_Atom_Forge_Ref atom = freeze_client_forge_patch_set(self, &frame);
    LV2_Atom_Forge *forge = &self->forge;
    const FreezeURIs *uris = self->uris;

    CHECK_FORGE(lv2_atom_forge_key(forge, uris->patch_property));
    CHECK_FORGE(lv2_atom_forge_urid(forge, uris->freeze_db_path));
    CHECK_FORGE(lv2_atom_forge_key(forge, uris->patch_value));

    const size_t max_path_len = sizeof(self->forge_buffer) - 512;
    size_t path_len = strlen(db_path);
    if (path_len > max_path_len) {
        path_len = max_path_len;
        plugin_log_warn(self->logger, "DB path is too long to send to UI.");
    }

    CHECK_FORGE(lv2_atom_forge_string(forge, db_path, path_len));
    lv2_atom_forge_pop(forge, &frame);
    freeze_client_write(self, atom);
}

void freeze_client_set_mem_used(FreezeClient *self, size_t mem_used) {
    LV2_Atom_Forge_Frame frame;
    LV2_Atom_Forge_Ref atom = freeze_client_forge_patch_set(self, &frame);
    LV2_Atom_Forge *forge = &self->forge;
    const FreezeURIs *uris = self->uris;

    CHECK_FORGE(lv2_atom_forge_key(forge, uris->patch_property));
    CHECK_FORGE(lv2_atom_forge_urid(forge, uris->freeze_mem_used));
    CHECK_FORGE(lv2_atom_forge_key(forge, uris->patch_value));
    CHECK_FORGE(lv2_atom_forge_long(forge, mem_used));
    lv2_atom_forge_pop(forge, &frame);
    freeze_client_write(self, atom);
}

void freeze_client_clear_db(FreezeClient *self) {
    LV2_Atom_Forge *forge = &self->forge;
    const FreezeURIs *uris = self->uris;
    freeze_client_reset_forge(self);
    LV2_Atom_Forge_Frame frame;
    LV2_Atom_Forge_Ref atom = lv2_atom_forge_object(
        &self->forge, &frame, 0, self->uris->cmd_Command
    );

    CHECK_FORGE(atom);
    CHECK_FORGE(lv2_atom_forge_key(forge, uris->cmd_type));
    CHECK_FORGE(lv2_atom_forge_urid(forge, uris->freeze_cmd_clear_db));
    lv2_atom_forge_pop(forge, &frame);
    freeze_client_write(self, atom);
}



/***********************/
/* Receiving functions */
/***********************/

void freeze_client_on_event(FreezeClient *self, const LV2_Atom *atom) {
    const FreezeURIs *uris = self->uris;
    // Ensure atom is object.
    if (!lv2_atom_forge_is_object_type(&self->forge, atom->type)) {
        plugin_log_warn(self->logger, "Received unexpected non-object atom.");
        return;
    }

    const LV2_Atom_Object *obj = (LV2_Atom_Object *)atom;
    LV2_URID obj_type = obj->body.otype;

    if (obj_type == uris->patch_Get) {
        FREEZE_CLIENT_CALLBACKS_CALL(&self->callbacks, ON_GET, NULL);
        return;
    }

    if (obj_type == uris->patch_Set) {
        freeze_client_on_patch_set(self, obj);
        return;
    }

    if (obj_type == uris->cmd_Command) {
        freeze_client_on_command(self, obj);
        return;
    }

    plugin_log_warn(self->logger, "Received unexpected atom object type.");
}

static void freeze_client_on_patch_set(
        FreezeClient *self, const LV2_Atom_Object *obj) {
    const FreezeURIs *uris = self->uris;

    // Get patch:property.
    const LV2_Atom *property = NULL;
    lv2_atom_object_get(obj, uris->patch_property, &property, 0);
    if (property == NULL) {
        plugin_log_warn(
            self->logger, "Missing property in patch:Set message."
        );
        return;
    }
    LV2_URID prop_urid = ((const LV2_Atom_URID *)property)->body;

    // Get patch:value.
    const LV2_Atom *value = NULL;
    lv2_atom_object_get(obj, uris->patch_value, &value, 0);
    if (value == NULL) {
        plugin_log_warn(self->logger, "Missing value in patch:Set message.");
        return;
    }

    if (prop_urid == uris->freeze_recording_mode) {
        FreezeRecordingMode mode = ((const LV2_Atom_Int *)value)->body;
        FREEZE_CLIENT_CALLBACKS_CALL(&self->callbacks, ON_MODE, mode);
        return;
    }

    if (prop_urid == uris->freeze_db_path) {
        const char *path = LV2_ATOM_BODY_CONST(value);
        FREEZE_CLIENT_CALLBACKS_CALL(&self->callbacks, ON_PATH, path);
        return;
    }

    if (prop_urid == uris->freeze_mem_used) {
        size_t mem_used = ((const LV2_Atom_Long *)value)->body;
        FREEZE_CLIENT_CALLBACKS_CALL(&self->callbacks, ON_MEM_USED, mem_used);
        return;
    }

    plugin_log_warn(self->logger, "Unexpected property in patch:Set message.");
}

static void freeze_client_on_command(
        FreezeClient *self, const LV2_Atom_Object *obj) {
    const FreezeURIs *uris = self->uris;

    // Get cmd:type.
    const LV2_Atom *type = NULL;
    lv2_atom_object_get(obj, uris->cmd_type, &type, 0);
    if (type == NULL) {
        plugin_log_warn(
            self->logger, "Missing type in cmd:Command message."
        );
        return;
    }
    LV2_URID type_urid = ((const LV2_Atom_URID *)type)->body;

    if (type_urid == uris->freeze_cmd_clear_db) {
        FREEZE_CLIENT_CALLBACKS_CALL(&self->callbacks, ON_CLEAR_DB, NULL);
        return;
    }

    plugin_log_warn(self->logger, "Unexpected type in cmd:Command message.");
}



/*******************/
/* Other functions */
/*******************/

void freeze_client_set_logger(
        FreezeClient *self, const PluginLogger *logger) {
    self->logger = logger;
}

void freeze_client_destroy(FreezeClient *self) {
    freeze_client_callbacks_destroy(&self->callbacks);
}

#ifdef HEADER
    #define FREEZE_CLIENT_ADD_CALLBACK(self, ...) do { \
        FREEZE_CLIENT_CALLBACKS_ADD(&(self)->callbacks, __VA_ARGS__); \
    } while (0)
#endif
