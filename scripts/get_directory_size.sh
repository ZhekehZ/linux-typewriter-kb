#!/bin/sh

du -ck "$1" | tail -1 | cut -f1