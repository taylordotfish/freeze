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

// @guard GUI__PMODS__RECORD_BUTTONS_H

#include "record_buttons.h"
#include <stddef.h>

#ifdef HEADER
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
#endif

void record_buttons_pm_init(RecordButtonsPM *self) {
    self->mode = FREEZE_MODE_PLAYING;
    self->callback_context = NULL;
    self->on_change = NULL;
    self->sync_context = NULL;
    self->sync = NULL;
}

void record_buttons_pm_set_mode(
        RecordButtonsPM *self, FreezeRecordingMode mode) {
    self->mode = mode;
}

bool record_buttons_pm_is_record_enabled(const RecordButtonsPM *self) {
    return self->mode == FREEZE_MODE_PLAYING;
}

bool record_buttons_pm_is_stop_enabled(const RecordButtonsPM *self) {
    return self->mode == FREEZE_MODE_RECORDING;
}

void record_buttons_pm_on_record_click(RecordButtonsPM *self) {
    if (self->on_change != NULL) {
        self->on_change(self->callback_context, FREEZE_MODE_RECORDING);
    }
}

void record_buttons_pm_on_stop_click(RecordButtonsPM *self) {
    if (self->on_change != NULL) {
        self->on_change(self->callback_context, FREEZE_MODE_PLAYING);
    }
}

void record_buttons_pm_on_clear_click(RecordButtonsPM *self) {
    if (self->on_clear != NULL) {
        self->on_clear(self->callback_context);
    }
}

void record_buttons_pm_sync(RecordButtonsPM *self) {
    if (self->sync != NULL) {
        self->sync(self->sync_context);
    }
}

void record_buttons_pm_set_sync(
        RecordButtonsPM *self, void *context, void (*sync)(void *)) {
    self->sync_context = context;
    self->sync = sync;
}

void record_buttons_pm_set_callbacks(
        RecordButtonsPM *self, void *context,
        RecordButtonsPMOnChange on_change, RecordButtonsPMOnClear on_clear) {
    self->callback_context = context;
    self->on_change = on_change;
    self->on_clear = on_clear;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"

void record_buttons_pm_free(RecordButtonsPM *self) {
}
