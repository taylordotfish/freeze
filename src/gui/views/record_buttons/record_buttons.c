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

// @guard GUI__VIEWS__RECORD_BUTTONS_H

#include "record_buttons.h"
#include "record_buttons.priv.h"
#include "../style/style.h"

#ifdef HEADER
    #include "../widgets/widgets.h"
    #include "gui/pmods/pmods.h"
    #include <gtk/gtk.h>

    typedef struct RecordButtonsView {
        RecordButtonsPM *pmod;

        GtkWidget *box;
        IconButton *record_button;
        IconButton *stop_button;
        IconButton *clear_button;
    } RecordButtonsView;
#endif

void record_buttons_view_init(RecordButtonsView *self, RecordButtonsPM *pmod) {
    // Containing box
    self->box = gtk_hbox_new(true, FREEZE_GTK_SPACING);

    // Record button
    self->record_button = icon_button_new();
    icon_button_set_text(self->record_button, "Record");
    icon_button_set_icon(self->record_button, "media-record");
    gtk_box_pack_start(
        GTK_BOX(self->box), icon_button_widget(self->record_button),
        true, true, 0
    );
    g_signal_connect(
        icon_button_widget(self->record_button), "clicked",
        G_CALLBACK(on_record_click), self
    );

    // Stop button
    self->stop_button = icon_button_new();
    icon_button_set_text(self->stop_button, "Stop");
    icon_button_set_icon(self->stop_button, "media-playback-stop");
    gtk_box_pack_start(
        GTK_BOX(self->box), icon_button_widget(self->stop_button),
        true, true, 0
    );
    g_signal_connect(
        icon_button_widget(self->stop_button), "clicked",
        G_CALLBACK(on_stop_click), self
    );

    // Clear button
    self->clear_button = icon_button_new();
    icon_button_set_text(self->clear_button, "Clear");
    icon_button_set_icon(self->clear_button, "edit-delete");
    gtk_box_pack_start(
        GTK_BOX(self->box), icon_button_widget(self->clear_button),
        true, true, 0
    );
    g_signal_connect(
        icon_button_widget(self->clear_button), "clicked",
        G_CALLBACK(on_clear_click), self
    );

    // Presentation model
    self->pmod = pmod;
    record_buttons_pm_set_sync(pmod, self, record_buttons_view_sync);
    record_buttons_pm_sync(pmod);
}

GtkWidget *record_buttons_view_widget(const RecordButtonsView *self) {
    return self->box;
}

static void record_buttons_view_sync(void *context) {
    RecordButtonsView *self = context;
    gtk_widget_set_sensitive(
        icon_button_widget(self->record_button),
        record_buttons_pm_is_record_enabled(self->pmod)
    );
    gtk_widget_set_sensitive(
        icon_button_widget(self->stop_button),
        record_buttons_pm_is_stop_enabled(self->pmod)
    );
}

#pragma GCC diagnostic ignored "-Wunused-parameter"

void record_buttons_view_destroy(RecordButtonsView *self) {
}


/***********************/
/* GTK signal handlers */
/***********************/

static void on_record_click(GtkWidget *widget, void *context) {
    RecordButtonsView *self = context;
    record_buttons_pm_on_record_click(self->pmod);
}

static void on_stop_click(GtkWidget *widget, void *context) {
    RecordButtonsView *self = context;
    record_buttons_pm_on_stop_click(self->pmod);
}

static void on_clear_click(GtkWidget *widget, void *context) {
    RecordButtonsView *self = context;
    record_buttons_pm_on_clear_click(self->pmod);
}
