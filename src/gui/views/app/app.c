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

// @guard FREEZE__GUI__VIEWS__APP_H

#include "app.h"
#include "../style/style.h"

#ifdef HEADER
    #include "../record_buttons/record_buttons.h"
    #include "../widgets/path_display/path_display.h"
    #include "../../presentation/app/app.h"
    #include <gtk/gtk.h>

    typedef struct AppView {
        RecordButtonsView record_buttons;
        PathDisplay db_path;
        GtkWidget *box;
        GtkWidget *path_label;
        GtkWidget *memory_label;
    } AppView;
#endif

void app_view_init(AppView *self) {
    // Containing box
    self->box = gtk_vbox_new(false, FREEZE_GTK_SPACING);
    gtk_widget_set_size_request(self->box, FREEZE_GTK_MAIN_MIN_WIDTH, -1);
    gtk_container_set_border_width(
        GTK_CONTAINER(self->box), FREEZE_GTK_MAIN_PADDING
    );

    // RecordButtonsView
    record_buttons_view_init(&self->record_buttons);
    gtk_box_pack_start(
        GTK_BOX(self->box), record_buttons_view_widget(&self->record_buttons),
        true, true, 0
    );

    // Database path
    path_display_init(&self->db_path, "Database Path:");
    gtk_box_pack_start(
        GTK_BOX(self->box), path_display_widget(&self->db_path),
        false, false, 0
    );

    // "Memory Used" label
    self->memory_label = gtk_label_new(NULL);
    gtk_box_pack_start(
        GTK_BOX(self->box), self->memory_label, false, false, 0
    );
    gtk_misc_set_alignment(GTK_MISC(self->memory_label), 0, 0);
}

GtkWidget *app_view_widget(const AppView *self) {
    return self->box;
}

void app_view_update(AppView *self, const AppPres *pres) {
    if (app_pres_memory_text_changed(pres)) {
        char *str = app_pres_make_memory_text(pres);
        gtk_label_set_text(GTK_LABEL(self->memory_label), str);
        free(str);
    }
    if (app_pres_db_path_changed(pres)) {
        path_display_set(&self->db_path, app_pres_get_db_path(pres));
    }
    RecordButtonsPres rb_pres = app_pres_get_record_buttons(pres);
    record_buttons_view_update(&self->record_buttons, &rb_pres);
}
