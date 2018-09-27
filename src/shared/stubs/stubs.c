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

// @guard SHARED__STUBS_H

#include "stubs.h"
#include "stubs.priv.h"
#include "utils/memory/memory.h"
#include <string.h>

#ifdef HEADER
    #include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#endif

#ifdef PRIVATE_HEADER
    #include "utils/array_list/array_list.h"

    typedef struct URIPair {
        const char *uri;
        LV2_URID urid;
    } URIPair;

    typedef struct StubURIDMap {
        ArrayList list;
        LV2_URID urid;
        LV2_URID_Map lv2_map;
    } StubURIDMap;
#endif

static void stub_urid_map_init(StubURIDMap *self) {
    array_list_init(&self->list, sizeof(URIPair));
    self->urid = 1;
    self->lv2_map = (LV2_URID_Map){
        .handle = self,
        .map = stub_map_func,
    };
}

static LV2_URID stub_map_func(LV2_URID_Map_Handle handle, const char *uri) {
    StubURIDMap *self = (StubURIDMap *)handle;
    for (size_t i = 0; i < array_list_length(&self->list); i++) {
        URIPair *pair = array_list_get(&self->list, i);
        if (strcmp(pair->uri, uri) == 0) {
            return pair->urid;
        }
    }
    URIPair *pair = array_list_append(&self->list);
    pair->uri = uri;
    pair->urid = self->urid++;
    return pair->urid;
}

LV2_URID_Map *freeze_stub_make_urid_map(void) {
    StubURIDMap *map = malloc_or_abort(sizeof(StubURIDMap));
    stub_urid_map_init(map);
    return &map->lv2_map;
}
