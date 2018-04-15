//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/styled_knobs.h"

const Km_Skin_Ptr &Styled_Knob_Default::style_skin()
{
    static Km_Skin_Ptr the_skin;
    if (!the_skin) {
        Km_Skin_Ptr skin(new Km_Skin);
        skin->load_data(BinaryData::knobskin_png, BinaryData::knobskin_pngSize, 64);
        the_skin = skin;
    }
    return the_skin;
}
