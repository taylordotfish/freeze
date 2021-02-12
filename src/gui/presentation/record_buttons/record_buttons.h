#ifndef FREEZE__GUI__PRESENTATION__RECORD_BUTTONS_H
#define FREEZE__GUI__PRESENTATION__RECORD_BUTTONS_H

#include <shared/client/client.h>
#include <shared/mode/mode.h>
#include <stdbool.h>

typedef struct RecordButtonsPres {
    FreezeRecordingMode mode;
    FreezeClient *client;
} RecordButtonsPres;

typedef struct RecordButtonsEvents {
    FreezeClient *client;
} RecordButtonsEvents;

bool record_buttons_pres_is_record_enabled(const RecordButtonsPres *self);

bool record_buttons_pres_is_stop_enabled(const RecordButtonsPres *self);

RecordButtonsEvents record_buttons_pres_events(const RecordButtonsPres *self);

void record_buttons_events_on_record_click(const RecordButtonsEvents *self);

void record_buttons_events_on_stop_click(const RecordButtonsEvents *self);

void record_buttons_events_on_clear_click(const RecordButtonsEvents *self);

#endif
