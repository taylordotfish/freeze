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

// @guard FREEZE__GUI__VIEWS__APP_H

#include "app.h"
#include "app.priv.h"
#include "../style/style.h"

#ifdef HEADER
    #include "../db_path/db_path.h"
    #include "../record_buttons/record_buttons.h"
    #include "gui/pmods/pmods.h"
    #include <gtk/gtk.h>

    typedef struct AppView {
        AppPM *pmod;
        DBPathView db_path;
        RecordButtonsView record_buttons;

        GtkWidget *box;
        GtkWidget *path_label;
        GtkWidget *memory_label;
    } AppView;
#endif

void app_view_init(AppView *self, AppPM *pmod) {
    // Containing box
    self->box = gtk_vbox_new(false, FREEZE_GTK_SPACING);
    gtk_widget_set_size_request(self->box, FREEZE_GTK_MAIN_MIN_WIDTH, -1);
    gtk_container_set_border_width(
        GTK_CONTAINER(self->box), FREEZE_GTK_MAIN_PADDING
    );

    // RecordButtonsView
    record_buttons_view_init(&self->record_buttons, &pmod->record_buttons);
    gtk_box_pack_start(
        GTK_BOX(self->box), record_buttons_view_widget(&self->record_buttons),
        true, true, 0
    );

    // DBPathView
    db_path_view_init(&self->db_path, &pmod->db_path);
    gtk_box_pack_start(
        GTK_BOX(self->box), db_path_view_widget(&self->db_path),
        false, false, 0
    );

    // "Memory Used" label
    self->memory_label = gtk_label_new(NULL);
    gtk_box_pack_start(
        GTK_BOX(self->box), self->memory_label, false, false, 0
    );
    gtk_misc_set_alignment(GTK_MISC(self->memory_label), 0, 0);

    // Presentation model
    self->pmod = pmod;
    app_pm_set_sync(pmod, self, app_view_sync);
    app_pm_sync(pmod);
}

GtkWidget *app_view_widget(const AppView *self) {
    return self->box;
}

static void app_view_sync(void *context) {
    AppView *self = context;
    char *str = app_pm_make_memory_text(self->pmod);
    gtk_label_set_text(GTK_LABEL(self->memory_label), str);
    free(str);
}

void app_view_destroy(AppView *self) {
    db_path_view_destroy(&self->db_path);
    record_buttons_view_destroy(&self->record_buttons);
}
