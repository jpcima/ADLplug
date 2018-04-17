//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/look_and_feel.h"

Custom_Look_And_Feel::Custom_Look_And_Feel()
{
#if 0 && VF_USE_FREETYPE
    initialize_fonts();
#endif
}

#if 0 && VF_USE_FREETYPE
Typeface::Ptr Custom_Look_And_Feel::getTypefaceForFont(Font const &font)
{
    Typeface::Ptr tf;
    String face_name = font.getTypefaceName();
    if (face_name == Font::getDefaultSansSerifFontName()) {
        Font f(font);
        f.setTypefaceName("FreeSans");
        tf = vf::FreeTypeFaces::createTypefaceForFont(f);
    }
    if (!tf)
        tf = LookAndFeel::getTypefaceForFont(font);
    return tf;
}

void Custom_Look_And_Feel::initialize_fonts()
{
    vf::FreeTypeFaces::addFaceFromMemory(7.0, 12.0, true, BinaryData::FreeSans_ttf, BinaryData::FreeSans_ttfSize);
    vf::FreeTypeFaces::addFaceFromMemory(7.0, 12.0, true, BinaryData::FreeSansBold_ttf, BinaryData::FreeSansBold_ttfSize);
}
#endif
