/*
 * Copyright (C) 2018, 2021 taylor.fish <contact@taylor.fish>
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

// @guard FREEZE__GUI__PRESENTATION__RECORD_BUTTONS_H

#include "record_buttons.h"
#include <stddef.h>

#ifdef HEADER
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
#endif

bool record_buttons_pres_is_record_enabled(const RecordButtonsPres *self) {
    return self->mode == FREEZE_MODE_PLAYING;
}

bool record_buttons_pres_is_stop_enabled(const RecordButtonsPres *self) {
    return self->mode == FREEZE_MODE_RECORDING;
}

RecordButtonsEvents record_buttons_pres_events(const RecordButtonsPres *self) {
    return (RecordButtonsEvents){
        .client = self->client,
    };
}

/*******************************/
/* RecordButtonsEvents methods */
/*******************************/

void record_buttons_events_on_record_click(const RecordButtonsEvents *self) {
    freeze_client_set_mode(self->client, FREEZE_MODE_RECORDING);
    freeze_client_write(self->client);
}

void record_buttons_events_on_stop_click(const RecordButtonsEvents *self) {
    freeze_client_set_mode(self->client, FREEZE_MODE_PLAYING);
    freeze_client_write(self->client);
}

void record_buttons_events_on_clear_click(const RecordButtonsEvents *self) {
    freeze_client_clear_db(self->client);
    freeze_client_write(self->client);
}
