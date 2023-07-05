#!/bin/bash
GNSSTK_APPS_VER=__VER__

# User created file that overwrites GNSSTK_APPS_VER variable.
# Use kv pair GNSSTK_APPS_VER=<major_version> in file /etc/toolkits.conf to overwrite.
# By default, /etc/toolkits.conf file doesn't exist so this gets ignored. 
FILE=/etc/toolkits.conf

if [ -f "$FILE" ]; then
    echo "found $FILE so sourcing it"
    set -o allexport && source $FILE && set +o allexport
fi

echo "gnsstk-apps major version being used is $GNSSTK_APPS_VER"
source /usr/share/gnsstk__VER__-apps/enable