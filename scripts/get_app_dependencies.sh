#!/bin/sh

for app in "$@"
do
    if [ ! -x "${app}" ]
    then
        continue
    fi

    LIBS="$( objdump -p "$app"           \
           | grep NEEDED                 \
           | sed -E 's/^\s+NEEDED\s+//g' \
           )"

    DEP="$(
        echo "$LIBS" | while IFS= read -r lib
        do
            dpkg -S "$lib" | head -n1 | sed -E 's/^([^:]+):.*/\1/g'
        done
    )"

    DEPS="${DEPS}${DEP}\n"
done

printf "$DEPS" | sort -u | sed -zE 's/\n([^$])/, \1/g' 