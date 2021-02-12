#ifndef FREEZE__PLUGIN__RECORDING_H
#define FREEZE__PLUGIN__RECORDING_H

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

void recording_init(Recording *self);

void recording_clear(Recording *self);

void recording_get(
    const Recording *self,
    size_t pos,
    size_t len,
    StereoPort out
);

void recording_set(Recording *self, size_t pos, StereoSlice samples);

void recording_save_db(Recording *self, const char *path);

bool recording_load_db(Recording *self, const char *path);

size_t recording_get_memory_used(const Recording *self);

void recording_set_logger(Recording *self, const PluginLogger *logger);

void recording_destroy(Recording *self);

#endif
