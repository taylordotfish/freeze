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

/** @file string.h
 * Null-terminated string utilities.
 */

// @guard FREEZE__UTILS__STRING_H

#include "string.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#ifdef HEADER
    #include <stdarg.h>
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdlib.h>
#endif

/**
 * Provides the functionality of strdup(), but calls abort() if the requested
 * memory could not be allocated.
 *
 * @param[in] str The string to copy.
 * @return The copied string. This should be freed with free().
 */
char *strdup_or_abort(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    char *result = malloc(strlen(str) + 1);
    if (result == NULL) {
        fprintf(stderr, "strdup(): Memory allocation failed.\n");
        abort();
    }
    strcpy(result, str);
    return result;
}

/**
 * Like vsprintf(), but returns the result as a heap-allocated string.
 *
 * @param[in] format The format string.
 * @param args Arguments for vsprintf().
 * @return The formatted string. This must be freed with free().
 */
char *vsprintf_heap(const char *format, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);
    size_t length = vsnprintf(NULL, 0, format, args);

    char *result = malloc(length + 1);
    if (result == NULL) {
        fprintf(stderr, "vsprintf_heap(): Memory allocation failed.\n");
        abort();
    }
    vsnprintf(result, length + 1, format, args_copy);
    va_end(args_copy);
    return result;
}

/**
 * Like sprintf(), but returns the result as a heap-allocated string.
 *
 * @param[in] format The format string.
 * @return The formatted string. This must be freed with free().
 */
char *sprintf_heap(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = vsprintf_heap(format, args);
    va_end(args);
    return result;
}

#ifdef HEADER
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
