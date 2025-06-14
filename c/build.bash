#!/bin/bash

gcc -Iinclude src/main.c src/ether.c src/arp.c -o spoofer
if [ $? -eq 0 ]; then
    echo "Build successful!"
    if [ "$1" == "run" ]; then
        if [ "$EUID" -eq 0 ]; then
            echo "Running the spoofer..."
            ./spoofer
        else
            echo "Please run this script with sudo or as root to execute the spoofer."
            exit 1
        fi
        
    fi
else
    echo "Build failed!"
fi