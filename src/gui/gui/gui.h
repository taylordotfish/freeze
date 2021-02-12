#ifndef FREEZE__GUI__GUI_H
#define FREEZE__GUI__GUI_H

#include "../models/models.h"
#include "../views/views.h"
#include <shared/client/client.h>

typedef struct FreezeGUI {
    AppModel model;
    AppView view;
    FreezeClient *client;
} FreezeGUI;

void freeze_gui_init(FreezeGUI *self, FreezeClient *client);

GtkWidget *freeze_gui_widget(const FreezeGUI *self);

void freeze_gui_destroy(FreezeGUI *self);

#endif
