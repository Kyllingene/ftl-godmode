#!/bin/bash
set -e

if [ "$EUID" = 0 ]; then
    echo "Error: DO NOT run as root!"
    exit 1
fi

if [ "$1" = "install" ]; then
    echo "Installing FTL Godmode"
    
    echo "Compiling mod"
    gcc -shared -fPIC -ldl inject.c -o inject.so
    
    echo "Hooking FTL"
    cp ./FTL-modded ~/".local/share/Steam/steamapps/common/FTL Faster Than Light/FTL"

    echo -e "Done! Copy the following text and follow the installation instructions:\n"
    echo "modded $(realpath ./inject.so)"

elif [ "$1" = "uninstall" ]; then
    echo "Uninstalling FTL Godmode"

    echo "Unhooking FTL"
    cp ./FTL-unmodded ~/".local/share/Steam/steamapps/common/FTL Faster Than Light/FTL"

    echo "Done! Follow the uninstallation instructions to finish."
else
    echo "See README.md for (un)installation instructions."
fi
