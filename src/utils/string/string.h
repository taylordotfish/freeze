#ifndef UTILS__STRING_H
#define UTILS__STRING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * Provides the functionality of strdup(), but calls abort() if the requested
 * memory could not be allocated.
 *
 * @param[in] str The string to copy.
 * @return The copied string. This should be freed with free().
 */
char *strdup_or_abort(const char *str);

/**
 * Like vsprintf(), but returns the result as a heap-allocated string.
 *
 * @param[in] format The format string.
 * @param args Arguments for vsprintf().
 * @return The formatted string. This must be freed with free().
 */
char *vsprintf_heap(const char *format, va_list args);

/**
 * Like sprintf(), but returns the result as a heap-allocated string.
 *
 * @param[in] format The format string.
 * @return The formatted string. This must be freed with free().
 */
char *sprintf_heap(const char *format, ...);

/**
 * Checks whether or not a string is @c NULL or equivalent to the empty
 * string.
 *
 * @param[in] string The string.
 * @return Whether the string is @c NULL or the empty string.
 */
static inline bool string_null_or_empty(const char *string) {
    return string == NULL || *string == '\0';
}

#endif
