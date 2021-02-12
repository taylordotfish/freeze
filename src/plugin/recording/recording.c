/*
 * Copyright (C) 2018, 2021 taylor.fish <contact@taylor.fish>
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

// @guard FREEZE__PLUGIN__RECORDING_H

#include "recording.h"
#include "recording.priv.h"
#include <utils/int/int.h>
#include <utils/memory/memory.h>

#include <assert.h>
#include <inttypes.h>
#include <limits.h> /* @include */
#include <string.h>

#ifdef HEADER
    #include "../samples/samples.h"
    #include <shared/logger/logger.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>

    #define RECORDING_CHUNK_LENGTH 0x10000  // 65536
    #define RECORDING_NUM_CHUNKS 0x4000  // 16384

    typedef struct RecordingChunk {
        float *samples_left;
        float *samples_right;
        uint_least16_t file_pos;
        bool modified;
    } RecordingChunk;

    typedef struct Recording {
        RecordingChunk chunks[RECORDING_NUM_CHUNKS];
        size_t saved_chunks;
        size_t allocated_chunks;

        const PluginLogger *logger;
        size_t last_allocated;
        bool cleared;
    } Recording;
#endif

#if CHAR_BIT != 8
    #error Could not verify that "char" is 8 bits wide.
#elif !defined(__GNUC__) && !defined(__STDC_IEC_559__)
    #error Could not verify that all-zero-bit floats are 0.0 on this system.
#endif

// Chunks filled entirely with samples below this value are not saved.
#define RECORDING_SAMPLE_EPSILON 1e-6

#define RECORDING_NULL_FILE_POS ((1 << 16) - 1)

#define RECORDING_DB_MAGIC_NUM 0x467265657a654442
#define RECORDING_DB_MAGIC_NUM_SIZE 8

#define RECORDING_DB_VERSION 0
#define RECORDING_DB_VERSION_SIZE 4

#define RECORDING_DB_CHUNK_INDEX_SIZE 2
#define RECORDING_DB_CHUNK_SIZE ( \
    RECORDING_DB_CHUNK_INDEX_SIZE + \
    RECORDING_CHUNK_LENGTH * sizeof(float) * 2 \
)

#define RECORDING_DB_BODY_START ( \
    RECORDING_DB_MAGIC_NUM_SIZE + RECORDING_DB_VERSION_SIZE \
)
#define RECORDING_DB_CHUNK_POS(file_pos) ( \
    RECORDING_DB_BODY_START + RECORDING_DB_CHUNK_SIZE * file_pos \
)

static void recording_chunk_init(RecordingChunk *self) {
    *self = (RecordingChunk){
        .samples_left = NULL,
        .samples_right = NULL,
        .file_pos = RECORDING_NULL_FILE_POS,
        .modified = false
    };
}

static void recording_chunk_clear(RecordingChunk *self) {
    free(self->samples_left);
    free(self->samples_right);
    recording_chunk_init(self);
}

static float *alloc_samples(void) {
    float *samples = malloc_array_or_abort(
        RECORDING_CHUNK_LENGTH, sizeof(float)
    );
    memset(samples, 0, RECORDING_CHUNK_LENGTH * sizeof(float));
    return samples;
}

static inline bool recording_chunk_is_allocated(const RecordingChunk *self) {
    return self->samples_left != NULL;
}

static void recording_chunk_alloc(RecordingChunk *self) {
    if (!recording_chunk_is_allocated(self)) {
        self->samples_left = alloc_samples();
        self->samples_right = alloc_samples();
    }
}



/*************/
/* Recording */
/*************/

void recording_init(Recording *self) {
    recording_clear(self);
    self->logger = &plugin_logger_fallback;
}

void recording_clear(Recording *self) {
    self->cleared = true;
    self->saved_chunks = 0;
    self->allocated_chunks = 0;
    self->last_allocated = -1;
    for (size_t i = 0; i < RECORDING_NUM_CHUNKS; i++) {
        recording_chunk_clear(&self->chunks[i]);
    }
}

void recording_get(
    const Recording *self,
    size_t pos,
    size_t len,
    StereoPort out
) {
    const size_t max_length = (
        min_uint(pos + len, RECORDING_CHUNK_LENGTH * RECORDING_NUM_CHUNKS) -
        min_uint(pos, RECORDING_CHUNK_LENGTH * RECORDING_NUM_CHUNKS)
    );

    const RecordingChunk *chunk = &self->chunks[pos / RECORDING_CHUNK_LENGTH];
    size_t offset = pos % RECORDING_CHUNK_LENGTH;
    size_t written = 0;

    for (; written < max_length; written++, offset++) {
        if (offset >= RECORDING_CHUNK_LENGTH) {
            offset = 0;
            chunk++;
        }
        if (!recording_chunk_is_allocated(chunk)) {
            out.left[written] = 0;
            out.right[written] = 0;
        } else {
            out.left[written] = chunk->samples_left[offset];
            out.right[written] = chunk->samples_right[offset];
        }
    }

    for (; written < len; written++) {
        out.left[written] = 0;
        out.right[written] = 0;
    }
}

// Prepares the chunk for writing and returns whether or not the chunk is the
// most recently allocated chunk.
static bool recording_prepare_chunk(Recording *self, RecordingChunk *chunk) {
    size_t index = chunk - self->chunks;
    if (self->last_allocated == index) {
        return true;
    }

    if (recording_chunk_is_allocated(chunk)) {
        return false;
    }

    RecordingChunk *last_allocated = (
        self->last_allocated < RECORDING_NUM_CHUNKS ?
        &self->chunks[self->last_allocated] :
        NULL
    );

    if (last_allocated && !last_allocated->modified) {
        RecordingChunk tmp = *chunk;
        *chunk = *last_allocated;
        *last_allocated = tmp;
    } else {
        recording_chunk_alloc(chunk);
        self->allocated_chunks++;
    }
    self->last_allocated = index;
    return true;
}

void recording_set(Recording *self, size_t pos, StereoSlice samples) {
    const size_t length = samples.length;
    if (length <= 0) {
        return;
    }

    if (pos + length >= RECORDING_CHUNK_LENGTH * RECORDING_NUM_CHUNKS) {
        plugin_log_error(self->logger, "Exceeded recording buffer size.");
        return;
    }

    RecordingChunk *chunk = &self->chunks[pos / RECORDING_CHUNK_LENGTH];
    size_t offset = pos % RECORDING_CHUNK_LENGTH;
    bool is_last_allocated = recording_prepare_chunk(self, chunk);

    for (size_t written = 0; written < length; written++, offset++) {
        if (offset >= RECORDING_CHUNK_LENGTH) {
            offset = 0;
            chunk++;
            is_last_allocated = recording_prepare_chunk(self, chunk);
        }

        chunk->samples_left[offset] = samples.left[written];
        chunk->samples_right[offset] = samples.right[written];

        chunk->modified |= (
            !is_last_allocated |
            (samples.left[written] >= RECORDING_SAMPLE_EPSILON) |
            (samples.right[written] >= RECORDING_SAMPLE_EPSILON)
        );
    }
}

static void recording_create_db_fp(Recording *self, FILE *file) {
    write_int(file, RECORDING_DB_MAGIC_NUM, RECORDING_DB_MAGIC_NUM_SIZE);
    write_int(file, RECORDING_DB_VERSION, RECORDING_DB_VERSION_SIZE);
    recording_save_db_fp(self, file);
}

static bool recording_create_db(Recording *self, const char *path) {
    plugin_log_trace(self->logger, "Creating new database: %s", path);
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        plugin_log_error(self->logger, "Could not create database: %s", path);
        return false;
    }
    recording_create_db_fp(self, file);
    fclose(file);
    return true;
}

static void recording_save_db_chunk(
    Recording *self,
    FILE *file,
    size_t index
) {
    RecordingChunk *chunk = &self->chunks[index];
    if (!chunk->modified) {
        return;
    }

    if (chunk->file_pos == RECORDING_NULL_FILE_POS) {
        chunk->file_pos = self->saved_chunks++;
    }

    fseek(file, RECORDING_DB_CHUNK_POS(chunk->file_pos), SEEK_SET);
    write_int(file, index, RECORDING_DB_CHUNK_INDEX_SIZE);
    write_samples(file, chunk->samples_left);
    write_samples(file, chunk->samples_right);
    chunk->modified = false;
}

static void recording_save_db_fp(Recording *self, FILE *file) {
    for (size_t i = 0; i < RECORDING_NUM_CHUNKS; i++) {
        recording_save_db_chunk(self, file, i);
    }
}

void recording_save_db(Recording *self, const char *path) {
    plugin_log_trace(self->logger, "Saving database: %s", path);
    if (self->cleared) {
        recording_create_db(self, path);
        self->cleared = false;
        return;
    }

    FILE *file = fopen(path, "ab+");
    fseek(file, 0, SEEK_SET);
    if (recording_check_db_header(self, file)) {
        recording_save_db_fp(self, file);
    } else {
        plugin_log_trace(self->logger, "Creating new database: %s", path);
        recording_create_db_fp(self, file);
    }
    fclose(file);
}

#ifdef PRIVATE_HEADER
    typedef enum ReadStatus {
        READ_OK,
        READ_END,
        READ_ERROR,
    } ReadStatus;
#endif

static ReadStatus recording_load_db_chunk(Recording *self, FILE *file) {
    uint_fast64_t chunk_index = 0;
    if (!read_int(file, &chunk_index, RECORDING_DB_CHUNK_INDEX_SIZE)) {
        return READ_END;
    }

    if (chunk_index >= RECORDING_NUM_CHUNKS) {
        plugin_log_error(
            self->logger, "Invalid chunk index in database: %zu", chunk_index
        );
        // Write new database when saved.
        self->cleared = true;
        return READ_ERROR;
    }

    RecordingChunk *chunk = &self->chunks[chunk_index];
    recording_chunk_alloc(chunk);
    read_samples(file, chunk->samples_left);
    read_samples(file, chunk->samples_right);
    chunk->file_pos = self->saved_chunks++;
    return READ_OK;
}

static ReadStatus recording_check_db_header(Recording *self, FILE *file) {
    assert(self != NULL);
    uint_fast64_t magic_num = 0;
    if (!read_int(file, &magic_num, RECORDING_DB_MAGIC_NUM_SIZE)) {
        return READ_END;
    }
    if (magic_num != RECORDING_DB_MAGIC_NUM) {
        plugin_log_error(
            self->logger, "Invalid DB magic number: %"PRIxFAST64, magic_num
        );
        return READ_ERROR;
    }

    uint_fast64_t version = 0;
    if (!read_int(file, &version, RECORDING_DB_VERSION_SIZE)) {
        plugin_log_error(self->logger, "Could not read version from DB.");
        return READ_ERROR;
    }
    if (version != RECORDING_DB_VERSION) {
        plugin_log_error(
            self->logger, "Invalid DB version: %"PRIuFAST64, version
        );
        return READ_ERROR;
    }
    return READ_OK;
}

static bool recording_load_db_fp(Recording *self, FILE *file) {
    if (self->allocated_chunks > 0) {
        recording_clear(self);
    }

    switch (recording_check_db_header(self, file)) {
        case READ_OK: {
            break;
        }
        case READ_END: {
            plugin_log_warn(self->logger, "Loaded empty database.");
            return true;
        }
        default: {
            return false;
        }
    }

    ReadStatus status = READ_OK;
    while ((status = recording_load_db_chunk(self, file)) == READ_OK) {
    }

    self->allocated_chunks = self->saved_chunks;
    self->cleared = false;
    return status == READ_END;
}

bool recording_load_db(Recording *self, const char *path) {
    FILE *file = fopen(path, "rb");
    plugin_log_trace(self->logger, "Loading database: %s", path);
    if (file == NULL) {
        plugin_log_warn(self->logger, "Could not open database: %s", path);
        return false;
    }
    bool status = recording_load_db_fp(self, file);
    fclose(file);
    return status;
}

size_t recording_get_memory_used(const Recording *self) {
    size_t full_chunks = 0;
    for (size_t i = 0; i < RECORDING_NUM_CHUNKS; i++) {
        if (recording_chunk_is_allocated(&self->chunks[i])) {
            full_chunks++;
        }
    }
    return full_chunks * RECORDING_CHUNK_LENGTH * sizeof(float) * 2;
}

void recording_set_logger(Recording *self, const PluginLogger *logger) {
    self->logger = logger;
}

static void recording_free_samples(Recording *self) {
    for (size_t i = 0; i < RECORDING_NUM_CHUNKS; i++) {
        recording_chunk_clear(&self->chunks[i]);
    }
}

void recording_destroy(Recording *self) {
    recording_free_samples(self);
}



/*************************/
/* Serialization helpers */
/*************************/

static inline void write_int(
    FILE *file,
    uint_fast64_t value,
    size_t num_bytes
) {
    for (size_t i = num_bytes; i > 0; i--) {
        fputc((value >> (8 * (i - 1))) & 0xff, file);
    }
}

static inline bool read_int(
    FILE *file,
    uint_fast64_t *result,
    size_t num_bytes
) {
    uint_fast64_t value = 0;
    for (size_t i = 0; i < num_bytes; i++) {
        int chr = fgetc(file);
        if (chr == EOF) {
            return false;
        }
        value <<= 8;
        value |= (unsigned char)chr;
    }
    *result = value;
    return true;
}

static inline void write_samples(FILE *file, const float *samples) {
    fwrite(samples, sizeof(*samples), RECORDING_CHUNK_LENGTH, file);
}

static inline void read_samples(FILE *file, float *samples) {
    fread(samples, sizeof(*samples), RECORDING_CHUNK_LENGTH, file);
}
