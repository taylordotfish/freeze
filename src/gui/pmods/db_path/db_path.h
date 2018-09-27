#ifndef GUI__PMODS__DB_PATH_H
#define GUI__PMODS__DB_PATH_H

#include <gtk/gtk.h>

typedef struct DBPathPM {
    char *path;

    void *sync_context;
    void (*sync)(void *);
} DBPathPM;

void db_path_pm_init(DBPathPM *self);

const char *db_path_pm_get_path(const DBPathPM *self);

void db_path_pm_set_path(DBPathPM *self, const char *path);

void db_path_pm_sync(DBPathPM *self);

void db_path_pm_set_sync(DBPathPM *self, void *context, void (*sync)(void *));

void db_path_pm_free(DBPathPM *self);

#endif
