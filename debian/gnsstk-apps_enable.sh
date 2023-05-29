#!/bin/bash
GNSSTK_APPS_VER=__VER__

# User created file that overwrites GNSSTK_APPS_VER variable if it exists
FILE=/etc/gnsstk-apps.conf

if [ -f "$FILE" ]; then
    echo "found $FILE so sourcing it"
    set -o allexport && source $FILE && set +o allexport
fi

echo "gnsstk-apps major version being used is $GNSSTK_APPS_VER"
source /usr/share/gnsstk__VER__-apps/enable