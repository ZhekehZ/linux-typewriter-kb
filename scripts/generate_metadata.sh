#!/bin/sh

VERSION="$1"   && shift
UUID="$1"      && shift

sed "s/@VERSION@/${VERSION}/g" < "$( dirname "$0" )/extension/metadata.json" |\
sed "s/@UUID@/${UUID}/g"                                         
