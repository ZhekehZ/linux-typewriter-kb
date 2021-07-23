#!/bin/sh
DIR=$( dirname "$0" )

if [ "$DIR" = "/usr/bin" ]
then
    DIR="/opt/typewriter_keyboard"
fi

if [ "$( id -u )" -eq 0 ]
then
    echo "Run program as non-root!"
    exit 1
fi

if [ "$1" = "--fg" ]
then 
    sudo echo "Start in foreground"
    sudo "${DIR}"/kb_read | "${DIR}"/tw_snd
else 
    sudo echo "Start in background"
    sudo "${DIR}"/kb_read | "${DIR}"/tw_snd &
fi