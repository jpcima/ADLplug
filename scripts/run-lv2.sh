#!/bin/sh -e

test -z "$JALV" && JALV=jalv.gtk3

absdir() {
    old=`pwd`; cd "$1"; new=`pwd`; cd "$old"; echo "$new"
}

build_location="`dirname "$0"`/../build"
export LV2_PATH=`absdir "$build_location/lv2"`
exec "$JALV" "https://github.com/jpcima/ADLplug"
