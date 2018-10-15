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

// @guard GUI__PMODS__DB_PATH_H

#include "db_path.h"
#include "utils/string/string.h"

#ifdef HEADER
    #include <gtk/gtk.h>

    typedef struct DBPathPM {
        char *path;

        void *sync_context;
        void (*sync)(void *);
    } DBPathPM;
#endif

void db_path_pm_init(DBPathPM *self) {
    self->path = NULL;
    self->sync_context = NULL;
    self->sync = NULL;
}

const char *db_path_pm_get_path(const DBPathPM *self) {
    const char *path = self->path;
    return path == NULL ? "" : path;
}

void db_path_pm_set_path(DBPathPM *self, const char *path) {
    free(self->path);
    self->path = strdup_or_abort(path);
}

void db_path_pm_sync(DBPathPM *self) {
    if (self->sync != NULL) {
        self->sync(self->sync_context);
    }
}

void db_path_pm_set_sync(DBPathPM *self, void *context, void (*sync)(void *)) {
    self->sync_context = context;
    self->sync = sync;
}

void db_path_pm_destroy(DBPathPM *self) {
    free(self->path);
}
