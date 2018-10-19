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

static void stub_urid_map_init(StubURIDMap *self);

static LV2_URID stub_map_func(LV2_URID_Map_Handle handle, const char *uri);

static void stub_map_destroy(StubURIDMap *self);
