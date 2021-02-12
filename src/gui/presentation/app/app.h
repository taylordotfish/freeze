#ifndef FREEZE__GUI__PRESENTATION__APP_H
#define FREEZE__GUI__PRESENTATION__APP_H

#include "../record_buttons/record_buttons.h"
#include "../../models/app/app.h"
#include "shared/client/client.h"
#include <stdbool.h>

typedef struct AppPres {
    AppModel *model;
    FreezeClient *client;
} AppPres;

char *app_pres_make_memory_text(const AppPres *self);

bool app_pres_memory_text_changed(const AppPres *self);

const char *app_pres_get_db_path(const AppPres *self);

bool app_pres_db_path_changed(const AppPres *self);

RecordButtonsPres app_pres_get_record_buttons(const AppPres *self);

#endif
