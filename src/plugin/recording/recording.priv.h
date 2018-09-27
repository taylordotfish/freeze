static void recording_chunk_init(RecordingChunk *self);

static void recording_chunk_clear(RecordingChunk *self);

static float *alloc_samples(void);

static inline bool recording_chunk_is_allocated(const RecordingChunk *self);

static void recording_chunk_alloc(RecordingChunk *self);

static void recording_chunk_prepare_for_write(RecordingChunk *self);

static inline float flush_to_zero(float val);

static void recording_create_db_fp(Recording *self, FILE *file);

static void recording_create_db(Recording *self, const char *path);

static void recording_save_db_chunk(
    Recording *self, FILE *file, size_t index);

static void recording_save_db_fp(Recording *self, FILE *file);

static bool recording_load_db_chunk(Recording *self, FILE *file);

static bool recording_check_db_header(Recording *self, FILE *file);

static void recording_load_db_fp(Recording *self, FILE *file);

static void recording_free_samples(Recording *self);

static inline void write_int(
    FILE *file, uint_fast64_t value, size_t num_bytes);

static inline bool read_int(
    FILE *file, uint_fast64_t *result, size_t num_bytes);

static inline void write_samples(FILE *file, const float *samples);

static inline void read_samples(FILE *file, float *samples);
