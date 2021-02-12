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

// @guard FREEZE__GUI__VIEWS__WIDGETS__DB_PATH_H

#include "path_display.h"
#include "../../style/style.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef HEADER
    #include <gtk/gtk.h>

    typedef struct PathDisplay {
        GtkWidget *box;
        GtkWidget *label;
        GtkWidget *entry;
    } PathDisplay;
#endif

void path_display_init(PathDisplay *self, const char* label) {
    // Containing box
    self->box = gtk_hbox_new(false, FREEZE_GTK_SPACING);

    // Label
    self->label = NULL;
    if (label) {
        self->label = gtk_label_new(label);
        gtk_box_pack_start(GTK_BOX(self->box), self->label, false, false, 0);
    }

    // Entry
    self->entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(self->box), self->entry, true, true, 0);
    gtk_editable_set_editable(GTK_EDITABLE(self->entry), false);
}

void path_display_set(PathDisplay *self, const char* path) {
    gtk_entry_set_text(GTK_ENTRY(self->entry), path);
}

GtkWidget *path_display_widget(const PathDisplay *self) {
    return self->box;
}
