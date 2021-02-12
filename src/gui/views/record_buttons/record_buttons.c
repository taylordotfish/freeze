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

// @guard FREEZE__GUI__VIEWS__RECORD_BUTTONS_H

#include "record_buttons.h"
#include "../style/style.h"
#include "utils/unused/unused.h" /* @include */
#include "record_buttons.priv.h"

#ifdef HEADER
    #include "../widgets/widgets.h"
    #include "../../presentation/record_buttons/record_buttons.h"
    #include <gtk/gtk.h>
    #include <stdbool.h>

    typedef struct RecordButtonsView {
        RecordButtonsEvents events;
        bool events_init: 1;

        GtkWidget *box;
        IconButton record_button;
        IconButton stop_button;
        IconButton clear_button;
    } RecordButtonsView;
#endif

void record_buttons_view_init(RecordButtonsView *self) {
    // Containing box
    self->box = gtk_hbox_new(true, FREEZE_GTK_SPACING);

    // Record button
    icon_button_init(&self->record_button);
    icon_button_set_text(&self->record_button, "Record");
    icon_button_set_icon(&self->record_button, "media-record");
    gtk_box_pack_start(
        GTK_BOX(self->box), icon_button_widget(&self->record_button),
        true, true, 0
    );

    // Stop button
    icon_button_init(&self->stop_button);
    icon_button_set_text(&self->stop_button, "Stop");
    icon_button_set_icon(&self->stop_button, "media-playback-stop");
    gtk_box_pack_start(
        GTK_BOX(self->box), icon_button_widget(&self->stop_button),
        true, true, 0
    );

    // Clear button
    icon_button_init(&self->clear_button);
    icon_button_set_text(&self->clear_button, "Clear");
    icon_button_set_icon(&self->clear_button, "edit-delete");
    gtk_box_pack_start(
        GTK_BOX(self->box), icon_button_widget(&self->clear_button),
        true, true, 0
    );

    // `events` will be initialized later in `record_buttons_view_update`.
    self->events_init = false;
}

void record_buttons_view_update(
    RecordButtonsView *self,
    const RecordButtonsPres *pres
) {
    record_buttons_set_events(self, pres);
    gtk_widget_set_sensitive(
        icon_button_widget(&self->record_button),
        record_buttons_pres_is_record_enabled(pres)
    );
    gtk_widget_set_sensitive(
        icon_button_widget(&self->stop_button),
        record_buttons_pres_is_stop_enabled(pres)
    );
}

static void record_buttons_set_events(
    RecordButtonsView *self,
    const RecordButtonsPres *pres
) {
    self->events = record_buttons_pres_events(pres);
    if (self->events_init) {
        return;
    }
    self->events_init = true;
    g_signal_connect(
        icon_button_widget(&self->record_button), "clicked",
        G_CALLBACK(on_record_click), self
    );
    g_signal_connect(
        icon_button_widget(&self->stop_button), "clicked",
        G_CALLBACK(on_stop_click), self
    );
    g_signal_connect(
        icon_button_widget(&self->clear_button), "clicked",
        G_CALLBACK(on_clear_click), self
    );
}

GtkWidget *record_buttons_view_widget(const RecordButtonsView *self) {
    return self->box;
}



/***********************/
/* GTK signal handlers */
/***********************/

static void on_record_click(UNUSED GtkWidget *widget, void *context) {
    RecordButtonsView *self = context;
    record_buttons_events_on_record_click(&self->events);
}

static void on_stop_click(UNUSED GtkWidget *widget, void *context) {
    RecordButtonsView *self = context;
    record_buttons_events_on_stop_click(&self->events);
}

static void on_clear_click(UNUSED GtkWidget *widget, void *context) {
    RecordButtonsView *self = context;
    record_buttons_events_on_clear_click(&self->events);
}
