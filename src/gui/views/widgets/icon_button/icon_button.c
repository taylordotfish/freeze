/*
 * Copyright (C) 2018 taylor.fish <contact@taylor.fish>
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

// @guard FREEZE__GUI__VIEWS__WIDGETS__ICON_BUTTON_H

#include "icon_button.h"
#include "icon_button.priv.h"
#include "utils/memory/memory.h"
#include <stdbool.h>

#ifdef HEADER
    #include <gtk/gtk.h>

    typedef struct IconButton {
        GtkWidget *button;
        GtkWidget *align;
        GtkWidget *box;
        GtkWidget *image;
        GtkWidget *label;
    } IconButton;
#endif

void icon_button_init(IconButton *self) {
    self->box = gtk_hbox_new(false, 4);
    self->align = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add(GTK_CONTAINER(self->align), self->box);

    self->button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(self->button), self->align);

    self->image = NULL;
    self->label = NULL;
}

static void icon_button_remove_text(IconButton *self) {
    if (self->label != NULL) {
        gtk_widget_destroy(self->label);
        self->label = NULL;
    }
}

void icon_button_set_text(IconButton *self, const char *text) {
    if (text == NULL) {
        icon_button_remove_text(self);
        return;
    }
    if (self->label != NULL) {
        gtk_label_set_text(GTK_LABEL(self->label), text);
        return;
    }
    self->label = gtk_label_new(text);
    gtk_box_pack_end(GTK_BOX(self->box), self->label, false, false, 0);
}

static void icon_button_remove_icon(IconButton *self) {
    if (self->image != NULL) {
        gtk_widget_destroy(self->label);
        self->image = NULL;
    }
}

void icon_button_set_icon(IconButton *self, const char *icon_name) {
    if (icon_name == NULL) {
        icon_button_remove_icon(self);
        return;
    }
    if (self->image != NULL) {
        gtk_image_set_from_icon_name(
            GTK_IMAGE(self->image), icon_name, GTK_ICON_SIZE_BUTTON
        );
        return;
    }
    self->image = gtk_image_new_from_icon_name(
        icon_name, GTK_ICON_SIZE_BUTTON
    );
    gtk_box_pack_start(GTK_BOX(self->box), self->image, false, false, 0);
}

GtkWidget *icon_button_widget(const IconButton *self) {
    return self->button;
}
