#ifndef GUI__VIEWS__APP_H
#define GUI__VIEWS__APP_H

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

void app_view_init(AppView *self, AppPM *pmod);

GtkWidget *app_view_widget(const AppView *self);

void app_view_destroy(AppView *self);

#endif
