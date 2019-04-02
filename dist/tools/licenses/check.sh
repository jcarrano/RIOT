#!/bin/sh

# Copyright 2017 Kaspar Schleiser <kaspar@schleiser.de>
# Copyright 2014 Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
#
# This file is subject to the terms and conditions of the GNU Lesser
# General Public License v2.1. See the file LICENSE in the top level
# directory for more details.

: "${RIOTBASE:=$(cd $(dirname $0)/../../../; pwd)}"
cd $RIOTBASE

: "${RIOTTOOLS:=${RIOTBASE}/dist/tools}"
. "${RIOTTOOLS}"/ci/changed_files.sh

# customizable
CHECKROOT=$(dirname "${0}")
LICENSEDIR="${CHECKROOT}/patterns"
OUTPUT="${CHECKROOT}/out"
UNKNOWN="${OUTPUT}/unknown"
TMP="${CHECKROOT}/.tmp"

# Needed for compatibility with BSD sed
TAB_CHAR="$(printf '\t')"

# License files must contain a single line
line_count() {
    wc -l < "${1}"
}

# Get the file where licenses are collected
license_dest() {
    echo "${OUTPUT}/""$(basename "${LICENSE}")"
}

# prepare
ROOT=$(git rev-parse --show-toplevel)
LICENSES="${LICENSEDIR}/*"
EXIT_CODE=0

: ${ERROR_EXIT_CODE:=1}

# reset output dir
rm -fr "${OUTPUT}"
mkdir -p "${OUTPUT}"
for LICENSE in ${LICENSES}; do
    if [ "$(line_count "${LICENSE}")" != 1 ] ; then
        echo "License must be contained in a single line: ${LICENSE}"
        exit "${ERROR_EXIT_CODE}"
    fi
    echo -n '' > "$(license_dest "${LICENSE}")"
done

FILES=$(FILEREGEX='\.([sSch]|cpp)$' changed_files)

# categorize files
for FILE in ${FILES}; do
    FAIL=1
    head -100 "${ROOT}/${FILE}" | sed -e 's/[\/\*'"${TAB_CHAR}"']/ /g' -e 's/$/ /' | tr -d '\r\n' | sed -e 's/  */ /g' > "${TMP}"
    for LICENSE in ${LICENSES}; do
        if pcregrep -q -f "${LICENSE}" "${TMP}"; then
            echo "${FILE}" >> "$(license_dest "${LICENSE}")"
            FAIL=0
            break
        fi
    done
    if [ ${FAIL} = 1 ]; then
        echo "${FILE}" >> "${UNKNOWN}"
        echo "file has an unknown license header: '${FILE}'"
        EXIT_CODE=${ERROR_EXIT_CODE}
    fi
done

exit ${EXIT_CODE}
