#!/bin/bash -e

banks=adldata/*

# Compress
cat $banks > _all.wopl
zopfli _all.wopl

# Write dictionary
offset=0
for bank in $banks; do
    filename=`basename "$bank"`
    filename=`echo "$filename" | sed -r 's/^[0-9]{3} //'`
    size=`stat -c '%s' "$bank"`
    printf '%.8x' "$size" | xxd -r -p
    printf '%.8x' "$offset" | xxd -r -p
    printf '%s' "$filename"
    printf '%.2x' 0 | xxd -r -p
    offset=$((offset+size))
done
printf '%.8x' 0 | xxd -r -p

# Write file data
cat _all.wopl.gz

# Clean up
rm _all.wopl.gz _all.wopl
