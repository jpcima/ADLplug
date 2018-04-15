//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "ui/knob_component.h"
#include "ui/knobman_skin.h"

class Styled_Knob_Default : public Knob
{
public:
    Styled_Knob_Default()
        { set_skin(style_skin()); }
    static const Km_Skin_Ptr &style_skin();
};

class Styled_Knob_DefaultSmall : public Knob
{
public:
    Styled_Knob_DefaultSmall()
        { set_skin(style_skin()); }
    static const Km_Skin_Ptr &style_skin();
};
