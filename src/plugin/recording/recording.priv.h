static void recording_chunk_init(RecordingChunk *self);

static float *alloc_samples(void);

static inline bool recording_chunk_is_allocated(const RecordingChunk *self);

static void recording_chunk_alloc(RecordingChunk *self);

static void recording_chunk_destroy(RecordingChunk *self);

// Prepares the chunk for writing and returns whether or not the chunk is the
// most recently allocated chunk.
static bool recording_prepare_chunk(Recording *self, RecordingChunk *chunk);

static void recording_create_db_fp(Recording *self, FILE *file);

static bool recording_create_db(Recording *self, const char *path);

static void recording_save_db_chunk(
    Recording *self,
    FILE *file,
    size_t index
);

static void recording_save_db_fp(Recording *self, FILE *file);

typedef enum ReadStatus {
    READ_OK,
    READ_END,
    READ_ERROR,
} ReadStatus;

static ReadStatus recording_load_db_chunk(Recording *self, FILE *file);

static ReadStatus recording_check_db_header(Recording *self, FILE *file);

static bool recording_load_db_fp(Recording *self, FILE *file);

static inline void write_int(
    FILE *file,
    uint_fast64_t value,
    size_t num_bytes
);

static inline bool read_int(
    FILE *file,
    uint_fast64_t *result,
    size_t num_bytes
);

static inline void write_samples(FILE *file, const float *samples);

static inline void read_samples(FILE *file, float *samples);
