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

// @guard FREEZE__GUI__VIEWS__DB_PATH_H

#include "db_path.h"
#include "db_path.priv.h"
#include "../style/style.h"

#ifdef HEADER
    #include "gui/pmods/pmods.h"
    #include <gtk/gtk.h>

    typedef struct DBPathView {
        DBPathPM *pmod;

        GtkWidget *box;
        GtkWidget *label;
        GtkWidget *entry;
    } DBPathView;
#endif

void db_path_view_init(DBPathView *self, DBPathPM *pmod) {
    // Containing box
    self->box = gtk_hbox_new(false, FREEZE_GTK_SPACING);

    // Label
    self->label = gtk_label_new("Database Path:");
    gtk_box_pack_start(GTK_BOX(self->box), self->label, false, false, 0);

    // Entry
    self->entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(self->box), self->entry, true, true, 0);
    gtk_editable_set_editable(GTK_EDITABLE(self->entry), false);

    // Presentation model
    self->pmod = pmod;
    db_path_pm_set_sync(pmod, self, db_path_view_sync);
    db_path_pm_sync(pmod);
}

GtkWidget *db_path_view_widget(const DBPathView *self) {
    return self->box;
}

static void db_path_view_sync(void *context) {
    DBPathView *self = context;
    gtk_entry_set_text(
        GTK_ENTRY(self->entry), db_path_pm_get_path(self->pmod)
    );
}

#pragma GCC diagnostic ignored "-Wunused-parameter"

void db_path_view_destroy(DBPathView *self) {
}
