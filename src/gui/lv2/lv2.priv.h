static void write_atom(void *context, const LV2_Atom *atom);

static LV2UI_Handle instantiate(
    UNUSED const LV2UI_Descriptor *descriptor,
    UNUSED const char *plugin_uri,
    UNUSED const char *bundle_path,
    LV2UI_Write_Function write_function,
    LV2UI_Controller controller,
    LV2UI_Widget *widget,
    const LV2_Feature * const *features
);

static void port_event(
    LV2UI_Handle handle,
    UNUSED uint32_t port_index,
    UNUSED uint32_t buffer_size,
    uint32_t format,
    const void *buffer
);

static void cleanup(LV2UI_Handle handle);

static const LV2UI_Descriptor descriptor;
