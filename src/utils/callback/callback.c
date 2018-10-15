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

/** @file callback.h
 * Utilities for managing callbacks.
 */

// @guard UTILS__CALLBACK_H

#include "callback.h"
#include <stdio.h>

#ifdef HEADER
    #include "utils/array_list/array_list.h"

    /**
     * A callback with associated context.
     */
    typedef struct Callback {
        /** The callback function. */
        void (*func)();
        /** Context for the callback function. */
        void *context;
    } Callback;

    /**
     * A list of callbacks.
     */
    typedef struct CallbackList {
        /** The array list containing the Callback objects. */
        ArrayList list;
    } CallbackList;
#endif

/**
 * Initializes a new CallbackList.
 *
 * @param[out] self The CallbackList.
 */
void callback_list_init(CallbackList *self) {
    array_list_init(&self->list, sizeof(Callback));
}

/**
 * Adds a callback to the list.
 *
 * @param[in] self The CallbackList.
 * @param func The callback function.
 * @param[in] context Context for the callback function.
 */
void callback_list_add(CallbackList *self, void (*func)(), void *context) {
    Callback *callback = array_list_append(&self->list);
    *callback = (Callback){ .func = func, .context = context };
}

/**
 * Frees memory used by the list.
 *
 * @param[in] self The CallbackList.
 */
void callback_list_destroy(CallbackList *self) {
    array_list_destroy(&self->list);
}

#ifdef HEADER
    /**
     * Adds a callback to a CallbackList.
     *
     * @param[in] self A pointer to the CallbackList.
     * @param func A pointer to the callback function.
     * @param context A pointer to context for the callback function.
     */
    #define CALLBACK_LIST_ADD(self, func, context) do { \
        callback_list_add(self, (void (*)())(func), context); \
    } while (0)

    /**
     * Calls the callbacks in a CallbackList. Arguments after @c FuncPtrType
     * will be passed to each callback after the context object (which is
     * always the first parameter and is of type <code>void *</code>). Due to
     * technical limitations, there must be at least one additional parameter.
     *
     * @param[in] self A pointer to the CallbackList.
     * @param FuncPtrType The function pointer type to which each callback will
     *   be cast. For example, <code>void (*)(void *context, int, bool)</code>.
     */
    #define CALLBACK_LIST_CALL(self, FuncPtrType, ...) do { \
        size_t length = array_list_length(&(self)->list); \
        for (size_t i = 0; i < length; i++) { \
            Callback *callback = array_list_get(&(self)->list, i); \
            ((FuncPtrType)callback->func)(callback->context, __VA_ARGS__); \
        } \
    } while (0)
#endif
