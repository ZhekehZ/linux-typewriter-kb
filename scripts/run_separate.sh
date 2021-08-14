#!/bin/sh

FILENAME=$( basename "$0" )

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

KB_READ='<to eval>'
TW_SND='<to eval>'

TW_CONFIG_PIPE=/tmp/typewriter-kb-pipe
TW_LOG_FILE=/tmp/typewriter-kb-log.txt

read_with_redirected_input()
{
    if [ "$1" = true ]
    then
        pkexec "$KB_READ" --pipe | "$TW_SND" > $TW_LOG_FILE
    else
        sudo "$KB_READ" --pipe | "$TW_SND" > $TW_LOG_FILE
    fi

    rm $TW_CONFIG_PIPE
}

call_start()
{
    check_root
    if [ -p $TW_CONFIG_PIPE ]
    then
        echo 'Typewriter already running'
        exit 1
    else 
        if [ "$1" = false ]
        then
            sudo echo "Running in background ..."
        fi

        mkfifo $TW_CONFIG_PIPE
        touch $TW_LOG_FILE
        read_with_redirected_input "$1" &
        echo 'v50' > $TW_CONFIG_PIPE
        echo 'Done.'
    fi
}

call_setvolume()
{
    value=0
    if [ "$1" -gt 100 ] 
    then
        value=100
    else
        value="$1"
    fi

    if [ -e $TW_CONFIG_PIPE ] && [ -p $TW_CONFIG_PIPE ]
    then 
        echo "v$value" > $TW_CONFIG_PIPE
    else
        echo 'Start typewriter-kb first'
    fi
}

call_getvolume()
{
    if [ -e $TW_CONFIG_PIPE ] && [ -p $TW_CONFIG_PIPE ]
    then 
        echo 'g' > $TW_CONFIG_PIPE
        percent="$( tail -1 $TW_LOG_FILE )"
        echo "Current volume is ${percent}%"
    else
        echo 'Start typewriter-kb first'
    fi
    
}

call_stop()
{
    if [ -e $TW_CONFIG_PIPE ] && [ -p $TW_CONFIG_PIPE ]
    then 
        echo "exit" > $TW_CONFIG_PIPE
    else
        echo 'Start typewriter-kb first'
    fi
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