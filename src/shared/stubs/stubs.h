#ifndef FREEZE__SHARED__STUBS_H
#define FREEZE__SHARED__STUBS_H

#include <lv2/lv2plug.in/ns/ext/urid/urid.h>

LV2_URID_Map *freeze_stub_make_urid_map(void);

void freeze_stub_free_urid_map(LV2_URID_Map *map);

#endif
