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

// @guard SHARED__CLIENT__CALLBACKS_H

#include "callbacks.h"
#include <stdlib.h>

#ifdef HEADER
    #include "utils/callback/callback.h"
    #include "shared/mode/mode.h"

    // Sent by plugin to indicate state; sent by UI to request state change.
    typedef void (*FREEZE_CLIENT_ON_MODE)(void *, FreezeRecordingMode mode);
    typedef void (*FREEZE_CLIENT_ON_PATH)(void *, const char *path);
    typedef void (*FREEZE_CLIENT_ON_MEM_USED)(void *, size_t mem_used);

    // Commands.
    typedef void (*FREEZE_CLIENT_ON_CLEAR_DB)(void *, void *unused);

    // Sent by UI to request state.
    typedef void (*FREEZE_CLIENT_ON_GET)(void *, void *unused);

    typedef struct FreezeClientCallbacks {
        CallbackList ON_MODE;
        CallbackList ON_PATH;
        CallbackList ON_MEM_USED;
        CallbackList ON_CLEAR_DB;
        CallbackList ON_GET;
    } FreezeClientCallbacks;
#endif

void freeze_client_callbacks_init(FreezeClientCallbacks *self) {
    callback_list_init(&self->ON_MODE);
    callback_list_init(&self->ON_PATH);
    callback_list_init(&self->ON_MEM_USED);
    callback_list_init(&self->ON_CLEAR_DB);
    callback_list_init(&self->ON_GET);
}

void freeze_client_callbacks_destroy(FreezeClientCallbacks *self) {
    callback_list_destroy(&self->ON_MODE);
    callback_list_destroy(&self->ON_PATH);
    callback_list_destroy(&self->ON_MEM_USED);
    callback_list_destroy(&self->ON_CLEAR_DB);
    callback_list_destroy(&self->ON_GET);
}

#ifdef HEADER
    #define FREEZE_CLIENT_CALLBACKS_ADD(self, cb_type, func, context) do { \
        CALLBACK_LIST_ADD(&(self)->cb_type, func, context); \
    } while (0)

    #define FREEZE_CLIENT_CALLBACKS_CALL(self, cb_type, ...) do { \
        CALLBACK_LIST_CALL( \
            &(self)->cb_type, FREEZE_CLIENT_ ## cb_type, __VA_ARGS__ \
        ); \
    } while (0)
#endif
