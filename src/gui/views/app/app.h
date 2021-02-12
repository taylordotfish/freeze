#ifndef FREEZE__GUI__VIEWS__APP_H
#define FREEZE__GUI__VIEWS__APP_H

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

void app_view_init(AppView *self);

GtkWidget *app_view_widget(const AppView *self);

void app_view_update(AppView *self, const AppPres *pres);

#endif
