#ifndef SHARED__CLIENT__CLIENT_H
#define SHARED__CLIENT__CLIENT_H

#include "../callbacks/callbacks.h"
#include "shared/logger/logger.h"
#include "shared/uris/uris.h"

#include <lv2/lv2plug.in/ns/ext/atom/forge.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include <stdint.h>

typedef void (*AtomWriteFunc)(void *context, const LV2_Atom *atom);

typedef struct FreezeClient {
    FreezeClientCallbacks callbacks;

    const FreezeURIs *uris;
    AtomWriteFunc write;
    void *write_context;

    LV2_Atom_Forge forge;
    uint8_t forge_buffer[2048];
    const PluginLogger *logger;
} FreezeClient;

void freeze_client_init(
    FreezeClient *self, const FreezeURIs *uris, AtomWriteFunc write,
    void *write_context);

void freeze_client_request_state(FreezeClient *self);

void freeze_client_set_mode(FreezeClient *self, FreezeRecordingMode mode);

void freeze_client_set_db_path(FreezeClient *self, const char *db_path);

void freeze_client_set_mem_used(FreezeClient *self, size_t mem_used);

void freeze_client_clear_db(FreezeClient *self);

void freeze_client_on_event(FreezeClient *self, const LV2_Atom *atom);

void freeze_client_set_logger(
    FreezeClient *self, const PluginLogger *logger);

void freeze_client_destroy(FreezeClient *self);

#define FREEZE_CLIENT_ADD_CALLBACK(self, ...) do { \
    FREEZE_CLIENT_CALLBACKS_ADD(&(self)->callbacks, __VA_ARGS__); \
} while (0)

#endif
