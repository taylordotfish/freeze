#ifndef SHARED__CLIENT__CALLBACKS_H
#define SHARED__CLIENT__CALLBACKS_H

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

void freeze_client_callbacks_init(FreezeClientCallbacks *self);

void freeze_client_callbacks_destroy(FreezeClientCallbacks *self);

#define FREEZE_CLIENT_CALLBACKS_ADD(self, cb_type, func, context) do { \
    CALLBACK_LIST_ADD(&(self)->cb_type, func, context); \
} while (0)

#define FREEZE_CLIENT_CALLBACKS_CALL(self, cb_type, ...) do { \
    CALLBACK_LIST_CALL( \
        &(self)->cb_type, FREEZE_CLIENT_ ## cb_type, __VA_ARGS__ \
    ); \
} while (0)

#endif
