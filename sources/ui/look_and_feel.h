//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Custom_Look_And_Feel : public LookAndFeel_V4
{
public:
    Typeface::Ptr getTypefaceForFont(const Font &font) override;

private:
    static Typeface::Ptr getOrCreateFont(
        Typeface::Ptr &font, const char *data, unsigned size);

private:
    Typeface::Ptr fontSansRegular;
    Typeface::Ptr fontSansItalic;
    Typeface::Ptr fontSansBold;
    Typeface::Ptr fontSansBoldItalic;
    Typeface::Ptr fontSerifRegular;
    Typeface::Ptr fontSerifItalic;
    Typeface::Ptr fontSerifBold;
    Typeface::Ptr fontSerifBoldItalic;
    Typeface::Ptr fontMonoRegular;
    Typeface::Ptr fontMonoItalic;
    Typeface::Ptr fontMonoBold;
    Typeface::Ptr fontMonoBoldItalic;

};
