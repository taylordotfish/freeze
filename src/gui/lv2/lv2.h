#ifndef FREEZE__GUI__LV2_H
#define FREEZE__GUI__LV2_H

#include "../gui/gui.h"
#include "../models/models.h"
#include "../presentation/presentation.h"
#include "../views/views.h"
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

typedef struct FreezeLV2UI {
    FreezeGUI gui;
    FreezeURIs uris;
    FreezeClient client;
    PluginLogger logger;

    LV2UI_Write_Function write;
    LV2UI_Controller controller;
    LV2_Log_Logger lv2_logger;
} FreezeLV2UI;

const LV2UI_Descriptor *lv2ui_descriptor(uint32_t index);

#endif
