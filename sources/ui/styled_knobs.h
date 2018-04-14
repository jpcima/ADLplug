//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "ui/knob_component.h"

class Styled_Knob_Default : public Knob
{
public:
    Styled_Knob_Default()
        { load_skin_data(BinaryData::knobskin_png, BinaryData::knobskin_pngSize, 64); }
};
