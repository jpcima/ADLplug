//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/components/styled_knobs.h"
#include "resources.h"

RESOURCE(Res, knob_skin);
RESOURCE(Res, slider_skin);

Km_Skin_Ptr Styled_Knob_Default::skin_;

Km_Skin *Styled_Knob_Default::style_skin()
{
    if (!skin_) {
        Km_Skin_Ptr skin(new Km_Skin);
        skin->load_data(Res::knob_skin.data, Res::knob_skin.size, 64);
        skin_ = skin;
    }
    return skin_.get();
}

//------------------------------------------------------------------------------
Km_Skin_Ptr Styled_Knob_DefaultSmall::skin_;

Km_Skin *Styled_Knob_DefaultSmall::style_skin()
{
    if (!skin_)
        skin_ = Styled_Knob_Default::style_skin()->scaled(0.7);
    return skin_.get();
}

//------------------------------------------------------------------------------
Km_Skin_Ptr Styled_Slider_Default::skin_;

Km_Skin *Styled_Slider_Default::style_skin()
{
    if (!skin_) {
        Km_Skin_Ptr skin(new Km_Skin);
        skin->load_data(Res::slider_skin.data, Res::slider_skin.size, 64);
        skin->style = Km_LinearHorizontal;
        skin_ = skin;
    }
    return skin_.get();
}

//------------------------------------------------------------------------------
Km_Skin_Ptr Styled_Slider_DefaultSmall::skin_;

Km_Skin *Styled_Slider_DefaultSmall::style_skin()
{
    if (!skin_)
        skin_ = Styled_Slider_Default::style_skin()->scaled(0.5);
    return skin_.get();
}

//------------------------------------------------------------------------------
namespace Styled_Knobs
{
    void release_skins()
    {
        #define RELEASE_SKIN(T) Styled_##T::release_skin();
        EACH_STYLED_KNOB(RELEASE_SKIN);
        #undef RELEASE_SKIN
    }
};
