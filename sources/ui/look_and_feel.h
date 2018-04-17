//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Custom_Look_And_Feel : public LookAndFeel_V4
{
public:
    Custom_Look_And_Feel();
#if 0 && VF_USE_FREETYPE
    Typeface::Ptr getTypefaceForFont(Font const &font) override;
#endif

private:
#if 0 && VF_USE_FREETYPE
    void initialize_fonts();
#endif
};
