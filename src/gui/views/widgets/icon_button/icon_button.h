#ifndef GUI__VIEWS__WIDGETS__ICON_BUTTON_H
#define GUI__VIEWS__WIDGETS__ICON_BUTTON_H

#include <gtk/gtk.h>

typedef struct IconButton {
    GtkWidget *button;
    GtkWidget *align;
    GtkWidget *box;
    GtkWidget *image;
    GtkWidget *label;
} IconButton;

IconButton *icon_button_new(void);

void icon_button_set_text(IconButton *self, const char *text);

void icon_button_set_icon(IconButton *self, const char *icon_name);

GtkWidget *icon_button_widget(const IconButton *self);

#endif
