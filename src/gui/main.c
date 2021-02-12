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

// This is a test file used for debugging -- see lv2/lv2.c for the actual
// plugin initialization.

#include "gui/gui.h"
#include "shared/client/client.h"
#include "shared/logger/logger.h"
#include "shared/stubs/stubs.h"
#include "shared/uris/uris.h"
#include "utils/unused/unused.h"

#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/ext/atom/forge.h>

#include <gtk/gtk.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

static LV2_Atom_Forge forge;
static FreezeURIs uris;
static uint8_t forge_buffer[1024];

static void on_destroy(UNUSED GtkWidget *window, UNUSED void *context) {
    gtk_main_quit();
}

static void atom_write_func(void *context, UNUSED const LV2_Atom *atom) {
    FreezeClient *client = context;
    printf("UI sent atom.\n");
    int mode = rand() % 2;
    printf("Setting mode to %d.\n", mode);

    LV2_Atom_Forge_Frame frame;
    lv2_atom_forge_set_buffer(&forge, forge_buffer, sizeof(forge_buffer));
    lv2_atom_forge_object(&forge, &frame, 0, uris.patch_Set);
    lv2_atom_forge_key(&forge, uris.patch_property);
    lv2_atom_forge_urid(&forge, uris.freeze_recording_mode);
    lv2_atom_forge_key(&forge, uris.patch_value);
    lv2_atom_forge_int(&forge, mode);
    lv2_atom_forge_pop(&forge, &frame);
    freeze_client_on_event(client, (const LV2_Atom *)forge_buffer);
}

int main(int argc, char **argv) {
    #if FREEZE_DEBUG
        plugin_logger_fallback.debug = true;
    #endif

    LV2_URID_Map *map = freeze_stub_make_urid_map();
    freeze_uris_init(&uris, map);
    lv2_atom_forge_init(&forge, uris.map);

    FreezeClient client;
    freeze_client_init(&client, &uris, atom_write_func, &client);

    gtk_init(&argc, &argv);
    FreezeGUI gui;
    freeze_gui_init(&gui, &client);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_modal(GTK_WINDOW(window), true);
    gtk_window_set_title(GTK_WINDOW(window), "Freeze");

    gtk_container_add(GTK_CONTAINER(window), freeze_gui_widget(&gui));
    gtk_widget_show_all(window);
    gtk_window_present(GTK_WINDOW(window));
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    gtk_main();
    freeze_gui_destroy(&gui);
    freeze_client_destroy(&client);
    return 0;
}
