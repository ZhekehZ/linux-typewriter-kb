#!/bin/sh

DEB_PKG_SIZE="$1" && shift
DEPS=""

for app in "$@"
do
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

DEPS="$( printf "$DEPS" | sort -u | sed -zE 's/\n([^$])/, \1/g' )" 

sed "s/@APP_SIZE@/${DEB_PKG_SIZE}/g" < "$( dirname "$0" )/deb/DEBIAN/control" |\
sed "s/@MAINTAINER_NAME@/$( git config user.name )/g"                         |\
sed "s/@DEPENDENCIES@/${DEPS}/g"                                              |\
sed "s/@MAINTAINER_EMAIL@/$( git config user.email )/g"  