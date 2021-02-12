#ifndef FREEZE__GUI__VIEWS__WIDGETS__DB_PATH_H
#define FREEZE__GUI__VIEWS__WIDGETS__DB_PATH_H

#include <gtk/gtk.h>

typedef struct PathDisplay {
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *entry;
} PathDisplay;

void path_display_init(PathDisplay *self, const char* label);

void path_display_set(PathDisplay *self, const char* path);

GtkWidget *path_display_widget(const PathDisplay *self);

#endif
