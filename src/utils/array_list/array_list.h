#ifndef FREEZE__UTILS__ARRAY_LIST_H
#define FREEZE__UTILS__ARRAY_LIST_H

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

/**
 * Initializes a new ArrayList.
 *
 * @param[out] self The ArrayList.
 * @param elem_size The size of each element, as returned by @c sizeof.
 */
void array_list_init(ArrayList *self, size_t elem_size);

/**
 * Intializes a new ArrayList with the specified capacity.
 *
 * @param[out] self The ArrayList.
 * @param elem_size The size of each element, as returned by @c sizeof.
 * @param capacity The number of elements that the list should be able to store
 *   without resizing.
 */
void array_list_init_with_capacity(
    ArrayList *self, size_t elem_size, size_t capacity);

/**
 * Gets an item in the list.
 *
 * @param[in] self The ArrayList.
 * @param index The index of the element to retrieve.
 * @return A pointer to the element. The value referenced by this pointer may
 *   be modified, but the pointer is invalidated if any elements are added to
 *   or removed from the list.
 */
void *array_list_get(const ArrayList *self, size_t index);

/**
 * Appends an item to the end of the list.
 *
 * @param[in] self The ArrayList.
 * @return A pointer to space for the new element. This pointer should be used
 *   to set the value of the new element. Reading from this pointer before
 *   writing to it results in undefined behavior.
 */
void *array_list_append(ArrayList *self);

/**
 * Removes an item from the end of the list.
 *
 * @param[in] self The ArrayList.
 * @param[out] out If non-null, the removed item is copied to the the memory
 *   referenced by this pointer.
 */
void array_list_pop(ArrayList *self, void *out);

/**
 * Inserts an item into the list.
 *
 * @param[in] self The ArrayList.
 * @param index The index of the new element.
 * @return A pointer to space for the new element. Reading from this pointer
 *   before writing to it results in undefined behavior.
 */
void *array_list_insert(ArrayList *self, size_t index);

/**
 * Removes an item from the list.
 *
 * @param[in] self The ArrayList.
 * @param index The index of the element to remove.
 * @param[out] out If non-null, the removed item is copied to the memory
 *   referenced by this pointer.
 */
void array_list_remove(ArrayList *self, size_t index, void *out);

/**
 * Gets the length of the list.
 *
 * @param[in] self The ArrayList.
 * @return The length.
 */
size_t array_list_length(const ArrayList *self);

/**
 * Gets the underlying array managed by the list.
 *
 * @param[in] self The ArrayList.
 * @return A pointer to the first element in the array.
 */
void *array_list_get_array(const ArrayList *self);

/**
 * Clears the list of all items.
 *
 * @param[in] self The ArrayList.
 */
void array_list_clear(ArrayList *self);

/**
 * Frees memory used by the list. If the ArrayList structure itself was
 * allocated on the heap, it must be freed with free() after calling this
 * function.
 *
 * @param[in] self The ArrayList.
 */
void array_list_destroy(ArrayList *self);

#endif
