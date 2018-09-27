#ifndef PLUGIN__LV2_H
#define PLUGIN__LV2_H

#include "../plugin/plugin.h"
#include "shared/logger/logger.h"

typedef struct FreezeLV2 {
    FreezePlugin plugin;
    FreezeClient client;
    PluginLogger logger;
    LV2_Log_Logger lv2_logger;
    FreezeURIs uris;

    LV2_Atom_Sequence *notify_port;
    LV2_Atom_Forge notify_forge;
    LV2_Atom_Forge_Frame notify_frame;

    LV2_Atom_Sequence *control_port;
    float *in_left;
    float *in_right;
    float *out_left;
    float *out_right;
} FreezeLV2;

const LV2_Descriptor *lv2_descriptor(uint32_t index);

#endif
