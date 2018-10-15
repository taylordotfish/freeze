#ifndef GUI__PMODS__RECORD_BUTTONS_H
#define GUI__PMODS__RECORD_BUTTONS_H

#include "shared/mode/mode.h"
#include <stdbool.h>

typedef void (*RecordButtonsPMOnClear)(void *context);
typedef void (*RecordButtonsPMOnChange)(
    void *context, FreezeRecordingMode mode
);

typedef struct RecordButtonsPM {
    FreezeRecordingMode mode;

    void *callback_context;
    RecordButtonsPMOnChange on_change;
    RecordButtonsPMOnClear on_clear;

    void *sync_context;
    void (*sync)(void *context);
} RecordButtonsPM;

void record_buttons_pm_init(RecordButtonsPM *self);

void record_buttons_pm_set_mode(
    RecordButtonsPM *self, FreezeRecordingMode mode);

bool record_buttons_pm_is_record_enabled(const RecordButtonsPM *self);

bool record_buttons_pm_is_stop_enabled(const RecordButtonsPM *self);

void record_buttons_pm_on_record_click(RecordButtonsPM *self);

void record_buttons_pm_on_stop_click(RecordButtonsPM *self);

void record_buttons_pm_on_clear_click(RecordButtonsPM *self);

void record_buttons_pm_sync(RecordButtonsPM *self);

void record_buttons_pm_set_sync(
    RecordButtonsPM *self, void *context, void (*sync)(void *));

void record_buttons_pm_set_callbacks(
    RecordButtonsPM *self, void *context,
    RecordButtonsPMOnChange on_change, RecordButtonsPMOnClear on_clear);

void record_buttons_pm_destroy(RecordButtonsPM *self);

#endif
