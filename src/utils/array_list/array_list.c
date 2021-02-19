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

/** @file array_list.h
 * Implements an array list, a growable data structure with constant-time
 * access to its elements.
 */

/** @file array_list.priv.h
 * Private header file for @c array_list.c.
 */

// @guard FREEZE__UTILS__ARRAY_LIST_H

#include "array_list.h"
#include "array_list.priv.h"
#include <utils/memory/memory.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifdef HEADER
    #include <stdbool.h>
    #include <stddef.h>

    /**
     * An array list: a growable data structure with constant-time access to
     * its elements.
     */
    typedef struct ArrayList {
        /** Array of items. */
        char *data;
        /** Number of items in the array. */
        size_t length;
        /** Number of items that can be stored without resizing. */
        size_t capacity;
        /** Size of each item/element. */
        size_t elem_size;
    } ArrayList;
#endif

#define ARRAY_LIST_INITIAL_CAPACITY 8

/**
 * Resets element data. This function is called by array_list_init() and
 * array_list_clear().
 *
 * @param[out] self The ArrayList.
 */
static void array_list_reset(ArrayList *self) {
    self->data = NULL;
    self->length = 0;
    self->capacity = 0;
}

/**
 * Initializes a new ArrayList.
 *
 * @param[out] self The ArrayList.
 * @param elem_size The size of each element, as returned by @c sizeof.
 */
void array_list_init(ArrayList *self, size_t elem_size) {
    array_list_reset(self);
    self->elem_size = elem_size;
}

/**
 * Intializes a new ArrayList with the specified capacity.
 *
 * @param[out] self The ArrayList.
 * @param elem_size The size of each element, as returned by @c sizeof.
 * @param capacity The number of elements that the list should be able to store
 *   without resizing.
 */
void array_list_init_with_capacity(
    ArrayList *self,
    size_t elem_size,
    size_t capacity
) {
    array_list_init(self, elem_size);
    array_list_resize_to(self, capacity);
}

/**
 * Resizes the list to the specified capacity, unless its capacity is already
 * at least as large as the specified capacity.
 *
 * @param[in] self The ArrayList.
 * @param capacity The number of elements that the list should be able to store
 *   without resizing.
 */
static void array_list_resize_to(ArrayList *self, size_t capacity) {
    if (capacity <= self->capacity) {
        return;
    }
    self->data = realloc_increase(
        self->data, &self->capacity, self->elem_size, capacity - self->capacity
    );
}

/**
 * Doubles the capacity of the list if the list is full.
 *
 * @param[in] self The ArrayList.
 */
static void array_list_maybe_resize(ArrayList *self) {
    if (self->length < self->capacity) {
        return;
    }
    size_t increase = self->capacity;
    if (increase <= 0) {
        increase = ARRAY_LIST_INITIAL_CAPACITY;
    }
    self->data = realloc_increase(
        self->data, &self->capacity, self->elem_size, increase
    );
}

/**
 * Gets an item in the list.
 *
 * @param[in] self The ArrayList.
 * @param index The index of the element to retrieve.
 * @return A pointer to the element. The value referenced by this pointer may
 *   be modified, but the pointer is invalidated if any elements are added to
 *   or removed from the list.
 */
void *array_list_get(const ArrayList *self, size_t index) {
    assert(index < self->length);
    return &self->data[index * self->elem_size];
}

/**
 * Appends an item to the end of the list.
 *
 * @param[in] self The ArrayList.
 * @return A pointer to space for the new element. This pointer should be used
 *   to set the value of the new element. Reading from this pointer before
 *   writing to it results in undefined behavior.
 */
void *array_list_append(ArrayList *self) {
    array_list_maybe_resize(self);
    const size_t old_length = self->length++;
    return array_list_get(self, old_length);
}

/**
 * Removes an item from the end of the list.
 *
 * @param[in] self The ArrayList.
 * @param[out] out If non-null, the removed item is copied to the the memory
 *   referenced by this pointer.
 */
void array_list_pop(ArrayList *self, void *out) {
    assert(self->length > 0);
    self->length--;
    if (out != NULL) {
        memcpy(out, array_list_get(self, self->length), self->elem_size);
    }
}

/**
 * Inserts an item into the list.
 *
 * @param[in] self The ArrayList.
 * @param index The index of the new element.
 * @return A pointer to space for the new element. Reading from this pointer
 *   before writing to it results in undefined behavior.
 */
void *array_list_insert(ArrayList *self, size_t index) {
    assert(index <= self->length);
    const size_t elem_size = self->elem_size;
    array_list_append(self);
    for (size_t i = self->length - 1; i > index; i--) {
        memcpy(
            array_list_get(self, i), array_list_get(self, i - 1), elem_size
        );
    }
    return array_list_get(self, index);
}

/**
 * Removes an item from the list.
 *
 * @param[in] self The ArrayList.
 * @param index The index of the element to remove.
 * @param[out] out If non-null, the removed item is copied to the memory
 *   referenced by this pointer.
 */
void array_list_remove(ArrayList *self, size_t index, void *out) {
    assert(index < self->length);
    const size_t elem_size = self->elem_size;
    self->length--;
    if (out != NULL) {
        memcpy(out, array_list_get(self, index), elem_size);
    }

    const size_t length = self->length;
    for (size_t i = index; i < length; i++) {
        memcpy(
            array_list_get(self, i), array_list_get(self, i + 1), elem_size
        );
    }
}

/**
 * Gets the length of the list.
 *
 * @param[in] self The ArrayList.
 * @return The length.
 */
size_t array_list_length(const ArrayList *self) {
    return self->length;
}

/**
 * Gets the underlying array managed by the list.
 *
 * @param[in] self The ArrayList.
 * @return A pointer to the first element in the array.
 */
void *array_list_get_array(const ArrayList *self) {
    return self->data;
}

/**
 * Clears the list of all items.
 *
 * @param[in] self The ArrayList.
 */
void array_list_clear(ArrayList *self) {
    array_list_destroy(self);
    array_list_reset(self);
}

/**
 * Frees memory used by the list. If the ArrayList structure itself was
 * allocated on the heap, it must be freed with free() after calling this
 * function.
 *
 * @param[in] self The ArrayList.
 */
void array_list_destroy(ArrayList *self) {
    free(self->data);
}
