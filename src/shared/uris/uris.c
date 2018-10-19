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

// @guard FREEZE__SHARED__URIS_H

#include "uris.h"
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/patch/patch.h>
#include <lv2/lv2plug.in/ns/ext/time/time.h>

#ifdef HEADER
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
#endif

void freeze_uris_init(FreezeURIs *uris, LV2_URID_Map *map) {
    uris->map = map;
    uris->freeze_recording_mode = map->map(
        map->handle, FREEZE__recording_mode
    );
    uris->freeze_db_path = map->map(map->handle, FREEZE__db_path);
    uris->freeze_mem_used = map->map(map->handle, FREEZE__mem_used);
    uris->freeze_cmd_clear_db = map->map(map->handle, FREEZE__cmd_clear_db);

    uris->cmd_Command = map->map(map->handle, CMD__Command);
    uris->cmd_type = map->map(map->handle, CMD__type);

    uris->patch_Get = map->map(map->handle, LV2_PATCH__Get);
    uris->patch_Set = map->map(map->handle, LV2_PATCH__Set);
    uris->patch_property = map->map(map->handle, LV2_PATCH__property);
    uris->patch_value = map->map(map->handle, LV2_PATCH__value);
    uris->atom_eventTransfer = map->map(map->handle, LV2_ATOM__eventTransfer);
    uris->atom_Path = map->map(map->handle, LV2_ATOM__Path);
    uris->atom_Long = map->map(map->handle, LV2_ATOM__Long);
    uris->atom_Float = map->map(map->handle, LV2_ATOM__Float);
    uris->time_Position = map->map(map->handle, LV2_TIME__Position);
    uris->time_frame = map->map(map->handle, LV2_TIME__frame);
    uris->time_speed = map->map(map->handle, LV2_TIME__speed);
}
