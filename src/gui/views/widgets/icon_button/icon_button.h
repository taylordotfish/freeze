#ifndef FREEZE__GUI__VIEWS__WIDGETS__ICON_BUTTON_H
#define FREEZE__GUI__VIEWS__WIDGETS__ICON_BUTTON_H

#include <gtk/gtk.h>

typedef struct IconButton {
    GtkWidget *button;
    GtkWidget *align;
    GtkWidget *box;
    GtkWidget *image;
    GtkWidget *label;
} IconButton;

void icon_button_init(IconButton *self);

void icon_button_set_text(IconButton *self, const char *text);

void icon_button_set_icon(IconButton *self, const char *icon_name);

GtkWidget *icon_button_widget(const IconButton *self);

#endif
