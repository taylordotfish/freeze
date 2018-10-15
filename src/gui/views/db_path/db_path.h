#ifndef GUI__VIEWS__DB_PATH_H
#define GUI__VIEWS__DB_PATH_H

#include "gui/pmods/pmods.h"
#include <gtk/gtk.h>

typedef struct DBPathView {
    DBPathPM *pmod;

    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *entry;
} DBPathView;

void db_path_view_init(DBPathView *self, DBPathPM *pmod);

GtkWidget *db_path_view_widget(const DBPathView *self);

void db_path_view_destroy(DBPathView *self);

#endif
