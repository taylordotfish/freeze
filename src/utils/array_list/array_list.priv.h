/**
 * Resets element data. This function is called by array_list_init() and
 * array_list_clear().
 *
 * @param[out] self The ArrayList.
 */
static void array_list_reset(ArrayList *self);

/**
 * Resizes the list to the specified capacity, unless its capacity is already
 * at least as large as the specified capacity.
 *
 * @param[in] self The ArrayList.
 * @param capacity The number of elements that the list should be able to store
 *   without resizing.
 */
static void array_list_resize_to(ArrayList *self, size_t capacity);

/**
 * Doubles the capacity of the list if the list is full.
 *
 * @param[in] self The ArrayList.
 */
static void array_list_maybe_resize(ArrayList *self);
