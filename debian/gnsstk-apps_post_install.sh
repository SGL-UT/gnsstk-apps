#!/bin/bash

# This file should be sourced and not executed as bash script.  Ex. source /etc/share/gnsstk__VER__-apps/gnsstk-apps_post_install.sh

SRC_FILE=/usr/share/gnsstk__VER__-apps/gnsstk-apps_enable.sh
TGT_FILE=/etc/profile.d/gnsstk-apps_enable.sh

# Initally file has not been copied yet.
COPIED=0

echo -e "\nAttempting to copy $SRC_FILE to $TGT_FILE \n"

# Copying gnsstk-apps_enable.sh script to /etc/profile.d
if [[ -f "$SRC_FILE" && -f "$TGT_FILE" ]]; then
    exit_loop=0
    while [ $exit_loop -lt 1 ]
    do
    read -p "$TGT_FILE exits so do you want to overwrite it (y/n)?" yn
        case $yn in
            [yY])
                echo -e "\nOk. Overwriting..."
                cp -f $SRC_FILE $TGT_FILE
                COPIED=1
                exit_loop=1
                ;;
            [nN])
                echo -e "\nOk. Exiting..."
                exit_loop=1
                ;;
            *)
                echo -e "\ninvalid response: ${yn}.  Try again..."
                ;;
        esac
    done
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