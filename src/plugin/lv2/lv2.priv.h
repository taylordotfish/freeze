#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/state/state.h>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>

static void write_atom(void *context, const LV2_Atom *atom);

static LV2_Handle instantiate(
    UNUSED const LV2_Descriptor *descriptor,
    UNUSED double sample_rate,
    UNUSED const char *bundle_path,
    const LV2_Feature * const *features
);

static void activate(LV2_Handle instance);

static void deactivate(LV2_Handle instance);

static void connect_port(LV2_Handle instance, uint32_t port, void *data);

static bool check_atom(
    FreezeLV2 *self, LV2_Atom *atom, LV2_URID expected_urid,
    const char *name);

static void update_position(FreezeLV2 *self, const LV2_Atom_Object *obj);

static void handle_atom_event(FreezeLV2 *self, const LV2_Atom_Event *event);

static void run(LV2_Handle instance, uint32_t sample_count);

static void cleanup(LV2_Handle instance);

static LV2_State_Status save(
    LV2_Handle instance,
    LV2_State_Store_Function store,
    LV2_State_Handle handle,
    UNUSED uint32_t flags,
    const LV2_Feature * const *features
);

static LV2_State_Status restore(
    LV2_Handle instance,
    LV2_State_Retrieve_Function retrieve,
    LV2_State_Handle handle,
    UNUSED uint32_t flags,
    const LV2_Feature * const *features
);

static const void *extension_data(const char *uri);

static const LV2_Descriptor descriptor;
