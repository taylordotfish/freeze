#ifndef GUI__PMODS__APP_H
#define GUI__PMODS__APP_H

#include "gui/models/models.h"
#include "shared/client/client.h"
#include "../db_path/db_path.h"
#include "../record_buttons/record_buttons.h"

typedef struct AppPM {
    AppModel *model;
    FreezeClient *client;
    DBPathPM db_path;
    RecordButtonsPM record_buttons;

    void *sync_context;
    void (*sync)(void *context);
} AppPM;

void app_pm_init(AppPM *self, AppModel *model, FreezeClient *client);

char *app_pm_make_memory_text(const AppPM *self);

void app_pm_sync(AppPM *self);

void app_pm_set_sync(AppPM *self, void *context, void (*sync)(void *));

void app_pm_free(AppPM *self);

#endif
