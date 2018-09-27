#ifndef SHARED__URIS_H
#define SHARED__URIS_H

#include <lv2/lv2plug.in/ns/ext/urid/urid.h>

#define FREEZE_URI "https://taylor.fish/plugins/freeze"
#define FREEZE_URI_PREFIX FREEZE_URI "#"
#define FREEZE_UI_URI FREEZE_URI_PREFIX "ui"

#define FREEZE__recording_mode FREEZE_URI_PREFIX "recording-mode"
#define FREEZE__db_path FREEZE_URI_PREFIX "db-path"
#define FREEZE__mem_used FREEZE_URI_PREFIX "mem-used"
#define FREEZE__cmd_clear_db FREEZE_URI_PREFIX "cmd-clear-db"

#define CMD_URI "https://taylor.fish/lv2/cmd"
#define CMD_URI_PREFIX CMD_URI "#"

#define CMD__Command CMD_URI_PREFIX "Command"
#define CMD__type CMD_URI_PREFIX "type"

typedef struct FreezeURIs {
    LV2_URID_Map *map;
    LV2_URID freeze_recording_mode;
    LV2_URID freeze_db_path;
    LV2_URID freeze_mem_used;
    LV2_URID freeze_cmd_clear_db;

    LV2_URID cmd_Command;
    LV2_URID cmd_type;

    LV2_URID patch_Get;
    LV2_URID patch_Set;
    LV2_URID patch_property;
    LV2_URID patch_value;
    LV2_URID atom_eventTransfer;
    LV2_URID atom_Path;
    LV2_URID atom_Long;
    LV2_URID atom_Float;
    LV2_URID time_Position;
    LV2_URID time_frame;
    LV2_URID time_speed;
} FreezeURIs;

void freeze_uris_init(FreezeURIs *uris, LV2_URID_Map *map);

#endif
