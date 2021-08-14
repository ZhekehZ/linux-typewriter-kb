#!/bin/sh

VERSION="$1"   && shift
UUID="$1"      && shift

TEMPLATE_DIR="$( dirname "$0" )/../templates/extension"

sed "s/@VERSION@/${VERSION}/g" < "${TEMPLATE_DIR}/metadata.json" |\
sed "s/@UUID@/${UUID}/g"                                         
