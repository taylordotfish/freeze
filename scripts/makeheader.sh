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

if ! which autoheaders > /dev/null; then
    echo >&2 "Could not find autoheaders (is it installed?)."
    echo >&2 "See <https://git.taylor.fish/taylor.fish/autoheaders>."
    exit 1
fi

usage() {
    echo "Usage: makeheader.sh <c-file>"
    echo
    echo 'If "file.c" is given, this will create "file.h".'
    echo 'If "file.priv.h" exists before running this script,'
    echo 'it will be replaced with a private header.'
    exit 1
}

if [ $# -ne 1 ]; then
    usage
fi

script_dir=$(realpath "$(dirname "$0")")
ah_opts=(-c-I"$script_dir/../src")
base=${1%.c}

tmpfile=$(mktemp)
if ! autoheaders "$1" "${ah_opts[@]}" > "$tmpfile"; then
    rm -f "$tmpfile"
    exit 1
fi

mv "$tmpfile" "$base".h
chmod +rw "$base".h

if [ -f "$base".priv.h ]; then
    autoheaders "$1" -p "${ah_opts[@]}" > "$base".priv.h
fi
