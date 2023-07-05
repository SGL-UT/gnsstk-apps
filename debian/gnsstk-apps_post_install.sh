#!/bin/bash

# This file should be sourced and not executed as bash script.  Ex. source /etc/share/gnsstk__VER__-apps/gnsstk-apps_post_install.sh

SRC_FILE=/usr/share/gnsstk__VER__-apps/gnsstk-apps_enable.sh
TGT_FILE=/etc/profile.d/gnsstk-apps_enable.sh

# Set current major version that is being installed. 
SRC_GNSSTK_APPS_VER=__VER__

# Initally file has not been copied yet.
COPIED=0

# Attempting to copy gnsstk-apps_enable.sh script to /etc/profile.d
# It will copy for these two use cases:
#   1) target file doesn't exist in /etc/profile.d
#   2) target file already exists but target gnsstk-apps major version is older
#         than source file gnsstk-apps major version
if [[ -f "$SRC_FILE" && -f "$TGT_FILE" ]]; then
    TGT_GNSSTK_APPS_VER=$(sed -n -E -e 's/^GNSSTK_APPS_VER=([0-9]+).*/\1/p' $TGT_FILE)
    if [ $TGT_GNSSTK_APPS_VER -lt $SRC_GNSSTK_APPS_VER ]; then
        echo -e "\nCopying $SRC_FILE to $TGT_FILE"
        cp -f $SRC_FILE $TGT_FILE
        COPIED=1
    else
        echo -e "\nTarget gnsstk-apps major version $TGT_GNSSTK_APPS_VER is greater or equal to source gnsstk-apps major version $SRC_GNSSTK_APPS_VER"
        echo -e "Therefore, not copying $SRC_FILE to $TGT_FILE\n"
    fi
else
    if [[ -f "$SRC_FILE" && ! -f "$TGT_FILE" ]]; then
        echo -e "\nCopying $SRC_FILE to $TGT_FILE"
        cp $SRC_FILE $TGT_FILE
        COPIED=1
    fi
fi

if [[ -f "$TGT_FILE" && $COPIED -ne 0 ]]; then
    echo -e "\nSourcing $TGT_FILE with new enviroment variables"
    source $TGT_FILE
fi

echo -e "\nThe following env variables have been set or are already set"
env | grep 'LD_LIBRARY_PATH\|CPLUS_INCLUDE_PATH\|PYTHONPATH\|^PATH\|CMAKE_PREFIX_PATH'