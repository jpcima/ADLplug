//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "ui/components/knob_component.h"
#include "ui/utility/knobman_skin.h"

#define EACH_STYLED_KNOB(F, ...)                \
    F(Knob_Default, ##__VA_ARGS__)              \
    F(Knob_DefaultSmall, ##__VA_ARGS__)         \
    F(Slider_Default, ##__VA_ARGS__)            \
    F(Slider_DefaultSmall, ##__VA_ARGS__)

class Styled_Knob_Default : public Knob
{
public:
    Styled_Knob_Default() { set_skin(style_skin()); }
    static Km_Skin *style_skin();
    static void release_skin() { skin_ = nullptr; }
private:
    static Km_Skin_Ptr skin_;
};

//------------------------------------------------------------------------------
class Styled_Knob_DefaultSmall : public Knob
{
public:
    Styled_Knob_DefaultSmall() { set_skin(style_skin()); }
    static Km_Skin *style_skin();
    static void release_skin() { skin_ = nullptr; }
private:
    static Km_Skin_Ptr skin_;
};

//------------------------------------------------------------------------------
class Styled_Slider_Default : public Knob
{
public:
    Styled_Slider_Default() { set_skin(style_skin()); }
    static Km_Skin *style_skin();
    static void release_skin() { skin_ = nullptr; }
private:
    static Km_Skin_Ptr skin_;
};

//------------------------------------------------------------------------------
class Styled_Slider_DefaultSmall : public Knob
{
public:
    Styled_Slider_DefaultSmall() { set_skin(style_skin()); }
    static Km_Skin *style_skin();
    static void release_skin() { skin_ = nullptr; }
private:
    static Km_Skin_Ptr skin_;
};

//------------------------------------------------------------------------------
namespace Styled_Knobs
{
    void release_skins();
};
