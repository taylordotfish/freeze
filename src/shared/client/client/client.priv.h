static void freeze_client_reset_forge(FreezeClient *self);

static void freeze_client_write(
    FreezeClient *self, const LV2_Atom_Forge_Ref ref);

static LV2_Atom_Forge_Ref freeze_client_forge_patch_set(
    FreezeClient *self, LV2_Atom_Forge_Frame *frame);

static void freeze_client_on_patch_set(
    FreezeClient *self, const LV2_Atom_Object *obj);

static void freeze_client_on_command(
    FreezeClient *self, const LV2_Atom_Object *obj);
