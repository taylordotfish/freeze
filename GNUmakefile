# Copyright (C) 2018, 2021 taylor.fish <contact@taylor.fish>
#
# This file is part of Freeze.
#
# Freeze is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Freeze is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Freeze.  If not, see <http://www.gnu.org/licenses/>.

BUILD_DIR = build

# Set to 1 for debug mode. The compiled plugin may not be real-time-safe.
DEBUG = 0

# Set to 1 for standalone mode: the produced binaries can be executed directly
# (and will not work as LV2 plugins). This is useful only for development.
STANDALONE = 0

SHARED_OBJECTS = $(addsuffix .o,$(basename $(shell \
	find src/shared src/utils -type f -name '*.c' \
	! -path 'src/shared/stubs/*' \
)))

PLUGIN_BINARY = freeze.so
PLUGIN_OBJECTS = $(addsuffix .o,$(basename $(shell \
	find src/plugin -type f -name '*.c' ! -name main.c \
)))

UI_BINARY = freeze_ui.so
UI_OBJECTS = $(addsuffix .o,$(basename $(shell \
	find src/gui -type f -name '*.c' ! -name main.c \
)))



CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -fpic -MMD -MP -Isrc \
         -fvisibility=hidden
LDFLAGS = -shared -Wl,--no-undefined,--no-allow-shlib-undefined
LDLIBS =



ifeq ($(DEBUG), 1)
	CFLAGS += -Og -ggdb -DFREEZE_DEBUG=1
else
	CFLAGS += -O3 -flto -DNDEBUG
	LDFLAGS += -O3 -flto
endif

ifeq ($(STANDALONE), 1)
	LDFLAGS =
	PLUGIN_OBJECTS += src/plugin/main.o
	UI_OBJECTS += src/gui/main.o
	SHARED_OBJECTS += $(addsuffix .o,$(basename $(shell \
		find src/shared/stubs -type f -name '*.c' \
	)))
endif



SHARED_OBJECTS := $(addprefix $(BUILD_DIR)/,$(SHARED_OBJECTS))
PLUGIN_BINARY := $(addprefix $(BUILD_DIR)/,$(PLUGIN_BINARY))
PLUGIN_OBJECTS := $(addprefix $(BUILD_DIR)/,$(PLUGIN_OBJECTS))
UI_BINARY := $(addprefix $(BUILD_DIR)/,$(UI_BINARY))
UI_OBJECTS := $(addprefix $(BUILD_DIR)/,$(UI_OBJECTS))

ALL_BINARIES = $(PLUGIN_BINARY) $(UI_BINARY)
ALL_OBJECTS = $(SHARED_OBJECTS) $(PLUGIN_OBJECTS) $(UI_OBJECTS)

$(UI_BINARY): LDLIBS += $(shell pkg-config --libs gtk+-2.0)
$(UI_OBJECTS): CFLAGS += $(shell pkg-config --cflags gtk+-2.0) \
                         -Wno-error=deprecated-declarations

ALL_PLUGIN_OBJECTS = $(SHARED_OBJECTS) $(PLUGIN_OBJECTS)
ALL_UI_OBJECTS = $(SHARED_OBJECTS) $(UI_OBJECTS)



BUILD_SUBDIRS = $(sort $(dir $(ALL_OBJECTS)))
ALL_BUILD_DIRS = $(BUILD_DIR) $(BUILD_SUBDIRS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<



.PHONY: all
all: $(ALL_BINARIES)

$(PLUGIN_BINARY): $(ALL_PLUGIN_OBJECTS)
$(UI_BINARY): $(ALL_UI_OBJECTS)

$(ALL_BINARIES):
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(ALL_OBJECTS:.o=.d)

$(ALL_OBJECTS) $(ALL_BINARIES): | $(ALL_BUILD_DIRS)

$(ALL_BUILD_DIRS):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean-binaries
clean-binaries:
	rm -rf $(ALL_BINARIES)
