#ifndef GUI__VIEWS__RECORD_BUTTONS_H
#define GUI__VIEWS__RECORD_BUTTONS_H

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

void record_buttons_view_init(RecordButtonsView *self, RecordButtonsPM *pmod);

GtkWidget *record_buttons_view_widget(const RecordButtonsView *self);

void record_buttons_view_free(RecordButtonsView *self);

#endif
