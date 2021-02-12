#ifndef FREEZE__GUI__VIEWS__RECORD_BUTTONS_H
#define FREEZE__GUI__VIEWS__RECORD_BUTTONS_H

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

void record_buttons_view_init(RecordButtonsView *self);

void record_buttons_view_update(
    RecordButtonsView *self,
    const RecordButtonsPres *pres
);

GtkWidget *record_buttons_view_widget(const RecordButtonsView *self);

#endif
