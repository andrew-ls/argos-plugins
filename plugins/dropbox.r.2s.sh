#!/bin/sh

# Copyright (c) 2018 Andrew Steel <copyright@andrewsteel.net>
#
# SPDX-License-Identifier: MIT
#
# The MIT License:
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# dropbox.sh:
# Displays the current Dropbox status as an icon, with detailed information
# displayed in a dropdown menu.

cd "$(dirname "${0}")"


#===============================================================================
# Constants
#-------------------------------------------------------------------------------

DROPDOWN_ENABLED=''
ICONS_COLOR='light' # dark|light
ICONS_SIZE='32x32' # 16x16|32x32


#===============================================================================
# Initialization
#-------------------------------------------------------------------------------

OUTPUT="$(
	test "$(command -v 'dropbox')" \
	&& printf '%s\n' "$(dropbox status)"
)"

# Determine the basic state:
#
# - "DONE":     Dropbox has finished synchronizing,
# - "OFFLINE":  No network connection,
# - "STARTING": Dropbox is initializing,
# - "SYNCING":  Dropbox is synchronizing,
# - "":         `dropbox` not in PATH, or unrecognized output.
STATE="$(
	case "$(
		printf '%s\n' "${OUTPUT}" \
		| head -n '1' \
		| cut -d ' ' -f '1'
	)" in
		('Checking')      echo 'STARTING';;
		('Connecting...') echo 'OFFLINE';;
		('Starting...')   echo 'STARTING';;
		('Syncing')       echo 'SYNCING';;
		('Syncing...')    echo 'SYNCING';;
		('Up')            echo 'DONE';;
		(*)               echo '';;
	esac
)"

BUTTON_ICON="$(
	case "${STATE}" in
		('DONE')     echo 'emblem-ok-symbolic';;
		('OFFLINE')  echo 'network-offline-symbolic';;
		('STARTING') echo 'content-loading-symbolic';;
		('SYNCING')  echo 'emblem-synchronizing-symbolic';;
		(*)          echo 'goa-panel-symbolic';;
	esac
)"

BUTTON_DROPDOWN="$(cat "$(pwd)/share/icons/${ICONS_SIZE:?}/pan-down-symbolic-${ICONS_COLOR:?}.svg.base64")"


#===============================================================================
# Render
#-------------------------------------------------------------------------------

printf '| iconName="%s" image="%s"\n---\n' \
	"${BUTTON_ICON}" \
	"${DROPDOWN_ENABLED:+${BUTTON_DROPDOWN}}"

	printf '%s\n' \
		"${OUTPUT}"