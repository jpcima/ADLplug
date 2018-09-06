#!/bin/bash -e

#ranges="U+0000-000FF"
#ranges="U+0000-000FF,U+2190–21FF"
ranges="U+0000-000FF,U+2190-21FF"

convert_font() {
    pyftsubset --unicodes="$ranges" --output-file="subset-$1" "$1"
    gzip -9 -c "subset-$1" > "$2"
    rm -f "subset-$1"
}

for style1 in Mono Sans Serif; do
    for style2 in Regular Bold Italic BoldItalic; do
        convert_font Liberation"$style1"-"$style2".ttf "$style1"-"$style2".ttf.gz
    done
done
