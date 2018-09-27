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

/** @file int.h
 * Integer manipulation utilities.
 */

#ifndef UTILS__INT_H
#define UTILS__INT_H

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Sets (to 1) every bit after the most significant bit in an integer.
 *
 * @param value The integer.
 * @return The resulting integer, where every bit after the most significant
 *   bit is 1.
 */
static inline uintmax_t fill_ones_after_msb(uintmax_t value) {
    for (size_t i = 1; i < sizeof(value) * CHAR_BIT; i *= 2) {
        value |= (value >> i);
    }
    return value;
}

/**
 * Calculates the smallest power of two that is greater than or equal to a
 * provided integer.
 *
 * @param value The integer.
 * @return The power of two.
 */
static inline uintmax_t pow2_ceil(uintmax_t value) {
    return fill_ones_after_msb(value - 1) + 1;
}

/**
 * Calculates the largest power of two that is less than or equal to a provided
 * integer.
 *
 * @param value The integer.
 * @return The power of two.
 */
static inline uintmax_t pow2_floor(uintmax_t value) {
    uintmax_t result = fill_ones_after_msb(value);
    return result - (result >> 1);
}

/**
 * Gets the minimum of two unsigned integers.
 *
 * @param first The first integer.
 * @param second The second integer.
 * @return The minimum integer.
 */
static inline uintmax_t min_uint(uintmax_t first, uintmax_t second) {
    return first < second ? first : second;
}

/**
 * Gets the maximum of two unsigned integers.
 *
 * @param first The first integer.
 * @param second The second integer.
 * @return The maximum integer.
 */
static inline uintmax_t max_uint(uintmax_t first, uintmax_t second) {
    return first > second ? first : second;
}

/**
 * Gets the minimum of two signed integers.
 *
 * @param first The first integer.
 * @param second The second integer.
 * @return The minimum integer.
 */
static inline intmax_t min_int(intmax_t first, intmax_t second) {
    return first < second ? first : second;
}

/**
 * Gets the maximum of two signed integers.
 *
 * @param first The first integer.
 * @param second The second integer.
 * @return The maximum integer.
 */
static inline intmax_t max_int(intmax_t first, intmax_t second) {
    return first > second ? first : second;
}

#endif
