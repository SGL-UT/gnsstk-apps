#!/bin/bash

# This file should be sourced and not executed as bash script.  Ex. source /etc/share/gnsstk__VER__-apps/gnsstk-apps_post_uninstall.sh

TGT_FILE=/etc/profile.d/gnsstk-apps_enable.sh

# Attempting to remove gnsstk-apps_enable.sh script from /etc/profile.d
if [-f "$TGT_FILE" ]; then
    exit_loop=0
    while [ $exit_loop -lt 1 ]
    do
    read -p "$TGT_FILE exists so do you want to delete it (y/n)?" yn
        case $yn in
            [yY])
                echo -e "\nOk. Removing file $TGT_FILE"
                rm -f $TGT_FILE
                if [ -f "$TGT_FILE" ]; then
                    echo -e "\n$TGT_FILE still exists so it will need to be manually removed."
                fi
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
fi