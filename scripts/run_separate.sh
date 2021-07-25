#!/bin/sh

FILENAME=$( basename "$0" )
KB_READ='<to eval>'
TW_SND='<to eval>'
TW_CONFIG='<to eval>'

call_start()
{
    check_root
    if [ "$1" = true ]
    then
        pkexec "$KB_READ" | "$TW_SND" &
    else
        sudo echo "Running in background ..."
        sudo "$KB_READ" | "$TW_SND" &
    fi

}

call_setvolume()
{
    $TW_CONFIG setvolume "$1"
}

call_getvolume()
{
    $TW_CONFIG getvolume
}

call_stop()
{
    $TW_CONFIG shutdown
}


display_help()
{
    HELP="
Usage: 
    ${FILENAME} start         -- start typewriter 
    ${FILENAME} stop          -- stop typewriter
    ${FILENAME} [OPTION]      -- configure typewriter

Options:
    -h, --help                show this help
    -s, --setvolume [VALUE]   set typewriter volume (from 0 to 100)
    -g, --getvolume           returns current typewriter volume 
 
    --gnome                   use gnome auth dialogue
"
    echo "$HELP"
}

evaluate_dirs()
{
    SCRIPT_DIR=$( dirname "$0" )
    if [ "$SCRIPT_DIR" = "/usr/bin" ]
    then
        SCRIPT_DIR="/opt/typewriter_keyboard"
    fi

    KB_READ="${SCRIPT_DIR}/kb_read"
    TW_SND="${SCRIPT_DIR}/tw_snd"
    TW_CONFIG="${SCRIPT_DIR}/tw_config"
}

check_root()
{
    if [ "$( id -u )" -eq 0 ]
    then
        echo "Run program as non-root!"
        exit 1
    fi
}

evaluate_dirs

mode= # start/stop/setvolume/getvolume
value= # volume for setvolume mode
selected_mods=0
use_gnome=false

assert_call_is_valid()
{
    if [ "$mode" = "setvolume" ] && [ -z "$value" ] || 
       [ -n "$1" ] || [ "$selected_mods" -ne 1 ]
    then
        echo "Invalid call" 
        display_help
        exit 1
    fi
}

while [ -n "$1" ]
do
    case "$1" in
        -h|--help)
            display_help
            exit
            ;;
        start)
            mode=start
            selected_mods=$(( selected_mods + 1 ))
            ;;
        stop)
            mode=stop
            selected_mods=$(( selected_mods + 1 ))
            ;;
        -s|--setvolume)
            value="$2"
            mode=setvolume
            shift
            selected_mods=$(( selected_mods + 1 ))
            ;;
        -g|--getvolume)
            mode=getvolume
            selected_mods=$(( selected_mods + 1 ))
            ;;
        --gnome)
            use_gnome=true
            ;;
        *) 
            echo "Invalid option: \`$1\`"
            display_help
            exit 1
            ;;
    esac
    shift
done

assert_call_is_valid "$1"


case "$mode" in
    start)     call_start "$use_gnome"
               ;;
    stop)      call_stop 
               ;;
    setvolume) call_setvolume "$value" 
               ;;
    getvolume) call_getvolume 
               ;;
esac