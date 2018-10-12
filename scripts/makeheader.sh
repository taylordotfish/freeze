#!/bin/bash
#
# makeheader.sh: Generates headers with autoheaders.
# See <https://git.taylor.fish/taylor.fish/autoheaders>.
#
# Copyright (C) 2018 taylor.fish <contact@taylor.fish>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set -e

NOT_FOUND_MSG='
Could not find autoheaders (is it installed?).
See <https://git.taylor.fish/taylor.fish/autoheaders>.
'

USAGE_MSG='
Usage: makeheader.sh <c-file>

If "file.c" is given, this will create "file.h". If "file.priv.h" exists
before running this script, it will be replaced with a private header.
'

VERSION_MSG='
autoheaders must be at least version 0.3.0. Please update autoheaders.
See <http://git.taylor.fish/taylor.fish/autoheaders>.
'

if ! which autoheaders > /dev/null; then
    echo >&2 -n "${NOT_FOUND_MSG:1}"
    exit 1
fi

usage() {
    echo -n "${USAGE_MSG:1}"
    exit 1
}

check-version() {
    local ah_version=$(autoheaders --version 2>/dev/null)
    local ah_major_version=$(grep -oP '^\d+' <<< "$ah_version")
    local ah_minor_version=$(grep -oP '^\d+\.\K\d+' <<< "$ah_version")
    [ "$ah_major_version" -gt 0 ] && return 0
    [ "$ah_minor_version" -ge 3 ] && return 0
    echo >&2 -n "${VERSION_MSG:1}"
    return 1
}

if [ $# -ne 1 ]; then
    usage
fi

base=${1%.c}
script_dir=$(realpath "$(dirname "$0")")
ah_opts=("$1" -c-I"$script_dir"/../src -o "$base".h)

if [ -f "$base".priv.h ]; then
    ah_opts+=(-p -o "$base".priv.h)
fi

if ! autoheaders "${ah_opts[@]}"; then
    check-version
    exit 1
fi
