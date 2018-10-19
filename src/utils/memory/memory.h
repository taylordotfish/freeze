#ifndef FREEZE__UTILS__MEMORY_H
#define FREEZE__UTILS__MEMORY_H

#include <stddef.h>
#include <stdlib.h>

/**
 * Calls realloc(), and then calls abort() if the requested memory could not be
 * allocated.
 *
 * @param data A pointer to the currently allocated memory, or @c NULL.
 * @param size The number of bytes to allocate.
 * @return A pointer to the newly allocated memory.
 */
void *realloc_or_abort(void *data, size_t size);

/**
 * Calls malloc(), and then calls abort() if the requested memory could not be
 * allocated.
 *
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory.
 */
void *malloc_or_abort(size_t size);

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
void *realloc_array_or_abort(void *data, size_t nmemb, size_t size);

/**
 * Allocates memory for an array, and calls abort() if the requested memory
 * could not be allocated or if too much memory was requested (more than
 * @c SIZE_MAX).
 *
 * @param nmemb The number of elements that the array should be able to hold.
 * @param size The size of each element.
 * @return A pointer to the allocated memory.
 */
void *malloc_array_or_abort(size_t nmemb, size_t size);

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
    void *data, size_t *nmemb, size_t size, size_t nmemb_inc);

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
size_t increase_alloc_size(size_t nmemb, size_t size, size_t nmemb_inc);

#endif
