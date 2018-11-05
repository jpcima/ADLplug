#!/bin/bash
set -e

for icon in ADLplug OPNplug; do
  for size in 32 96; do
    convert -resize "$size"x"$size" "$icon".png "$icon"-"$size".png
    optipng "$icon"-"$size".png
  done
done
