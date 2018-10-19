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

#ifndef FREEZE__SHARED__PORTS_H
#define FREEZE__SHARED__PORTS_H

typedef enum FreezeLV2Port {
    FREEZE_LV2_PORT_IN_L = 0,
    FREEZE_LV2_PORT_IN_R = 1,
    FREEZE_LV2_PORT_OUT_L = 2,
    FREEZE_LV2_PORT_OUT_R = 3,
    FREEZE_LV2_PORT_CONTROL = 4,
    FREEZE_LV2_PORT_NOTIFY = 5,
} FreezeLV2Port;

#endif
