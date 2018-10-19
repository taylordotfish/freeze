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

/** @file memory.h
 * Memory allocation utilities.
 */

// @guard FREEZE__UTILS__MEMORY_H

#include "memory.h"
#include <inttypes.h>
#include <stdio.h>

#ifdef HEADER
    #include <stddef.h>
    #include <stdlib.h>
#endif

/**
 * Calls realloc(), and then calls abort() if the requested memory could not be
 * allocated.
 *
 * @param data A pointer to the currently allocated memory, or @c NULL.
 * @param size The number of bytes to allocate.
 * @return A pointer to the newly allocated memory.
 */
void *realloc_or_abort(void *data, size_t size) {
    void *new_data = realloc(data, size);
    if (new_data == NULL && size > 0) {
        fprintf(stderr, "Memory allocation failed.\n");
        abort();
    }
    return new_data;
}

/**
 * Calls malloc(), and then calls abort() if the requested memory could not be
 * allocated.
 *
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory.
 */
void *malloc_or_abort(size_t size) {
    return realloc_or_abort(NULL, size);
}

/**
 * Reallocates memory for an array, and calls abort() if the requested memory
 * could not be allocated or if too much memory was requested (more than
 * @c SIZE_MAX).
 *
 * @param data A pointer to the currently allocated memory, or @c NULL.
 * @param nmemb The number of elements that the array should be able to hold.
 * @param size The size of each element.
 * @return A pointer to the newly allocated memory.
 */
void *realloc_array_or_abort(void *data, size_t nmemb, size_t size) {
    if (nmemb > SIZE_MAX / size) {
        fprintf(stderr, "Memory allocation failed (would exceed SIZE_MAX).\n");
        abort();
    }
    return realloc_or_abort(data, nmemb * size);
}

/**
 * Allocates memory for an array, and calls abort() if the requested memory
 * could not be allocated or if too much memory was requested (more than
 * @c SIZE_MAX).
 *
 * @param nmemb The number of elements that the array should be able to hold.
 * @param size The size of each element.
 * @return A pointer to the allocated memory.
 */
void *malloc_array_or_abort(size_t nmemb, size_t size) {
    return realloc_array_or_abort(NULL, nmemb, size);
}

/**
 * Increases the amount of memory allocated for an array, and calls abort() if
 * the requested memory could not be allocated or if too much memory was
 * requested (more than @c SIZE_MAX).
 *
 * @param data A pointer to the currently allocated memory.
 * @param[in,out] nmemb (A pointer to) the current number of elements that the
 *   array can hold. After increasing the amount of allocated memory, the new
 *   number of elements that the array can hold is written to this pointer.
 * @param size The size of each element.
 * @param nmemb_inc The number of additional members that the array should be
 *   able to hold. If the current amount of allocated memory is close to
 *   @c SIZE_MAX, the size of the array may not be increased by this many
 *   elements, but it will be increased by at least one element, or else
 *   abort() will be called. To see how many elements the array can hold after
 *   calling this function, read the value written to @c nmemb.
 * @return A pointer to the newly allocated memory.
 */
void *realloc_increase(
        void *data, size_t *nmemb, size_t size, size_t nmemb_inc) {
    const size_t new_nmemb = increase_alloc_size(*nmemb, size, nmemb_inc);
    if (new_nmemb <= 0) {
        fprintf(stderr, "Memory allocation failed (would exceed SIZE_MAX).\n");
        abort();
    }
    *nmemb = new_nmemb;
    // Multiplication will not overflow.
    return realloc_or_abort(data, new_nmemb * size);
}

/**
 * Calculates the increased size of a heap-allocated array (but does not
 * actually manipulate any memory).
 *
 * @param nmemb The number of elements that the array can currently hold.
 * @param size The size of each element.
 * @param nmemb_inc The number of additional members that the array should
 *   be able to hold. If the current amount of allocated memory is close to
 *   @c SIZE_MAX, the size of the array may not be increased by this many
 *   elements, but the largest possible size will be returned, unless the array
 *   cannot be grown at all.
 * @return The new number of elements that they array should be able to hold.
 *   When this value is multiplied by @c size, it is guaranteed not to
 *   overflow. If the array is already the maximum size, 0 is returned.
 */
size_t increase_alloc_size(size_t nmemb, size_t size, size_t nmemb_inc) {
    const size_t max_nmemb = SIZE_MAX / size;
    if (nmemb >= max_nmemb) {
        return 0;
    }

    nmemb_inc = (nmemb_inc > 0 ? nmemb_inc : 1);
    if (max_nmemb - nmemb < nmemb_inc) {
        return max_nmemb;
    }
    return nmemb + nmemb_inc;
}
