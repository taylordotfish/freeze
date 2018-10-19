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

#include "plugin/plugin.h"
#include "shared/client/client.h"
#include "shared/logger/logger.h"
#include "shared/stubs/stubs.h"
#include "shared/uris/uris.h"

#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <assert.h>
#include <stdio.h>

#define DB_PATH "FreezeDB-TEST"

#pragma GCC diagnostic ignored "-Wunused-parameter"

float output_left[1024];
float output_right[1024];
const StereoPort output = {
    .left = output_left,
    .right = output_right,
};

const float left1[] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
const float right1[] = { 0.9, 0.8, 0.7, 0.6, 0.5 };
const StereoSlice input1 = {
    .left = left1, .right = right1, .length = sizeof(left1) / sizeof(*left1),
};

const float left2[] = { 9e-7, 0, 2e-7, 9e-7 };
const float right2[] = { 2e-7, 1e-20, 0, 0 };
const StereoSlice input2 = {
    .left = left2, .right = right2, .length = sizeof(left2) / sizeof(*left2),
};

static void on_ui_atom_write(void *context, const LV2_Atom *atom) {
    printf("Sending atom to plugin...\n");
    FreezeClient *plugin_client = context;
    freeze_client_on_event(plugin_client, atom);
}

static void on_plugin_atom_write(void *context, const LV2_Atom *atom) {
    printf("Plugin sent atom.\n");
}

int main(void) {
    #if FREEZE_DEBUG
        plugin_logger_fallback.debug = true;
    #endif

    LV2_URID_Map *map = freeze_stub_make_urid_map();
    FreezeURIs uris;
    freeze_uris_init(&uris, map);

    FreezePlugin plugin;
    FreezeClient plugin_client;
    freeze_client_init(&plugin_client, &uris, on_plugin_atom_write, &plugin);

    FreezeClient ui_client;
    freeze_client_init(&ui_client, &uris, on_ui_atom_write, &plugin_client);

    freeze_plugin_init(&plugin, &plugin_client);
    freeze_plugin_set_db_path(&plugin, DB_PATH);
    freeze_plugin_load_db(&plugin);
    freeze_plugin_set_speed(&plugin, 1);
    freeze_client_set_mode(&ui_client, FREEZE_MODE_RECORDING);

    freeze_plugin_set_frame(&plugin, 100);
    freeze_plugin_run(&plugin, input1, output);

    freeze_plugin_set_frame(&plugin, 66000);
    freeze_plugin_run(&plugin, input2, output);

    freeze_plugin_save_db(&plugin);
    freeze_plugin_destroy(&plugin);
    freeze_client_destroy(&plugin_client);
    freeze_client_destroy(&ui_client);
    freeze_stub_free_urid_map(map);
    return 0;
}
