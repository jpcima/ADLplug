//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
#include <string>
#include <array>
class Configuration;

enum class Key_Layout {
    Qwerty, Azerty,
    Default = Qwerty,
};

extern const std::array<const char *, 2> key_layout_names;
extern const std::array<const char32_t *, 2> key_layout_maps;

Key_Layout set_key_layout(MidiKeyboardComponent &kb, Key_Layout layout, Configuration &conf);
Key_Layout load_key_configuration(MidiKeyboardComponent &kb, Configuration &conf);
void build_key_layout_menu(PopupMenu &menu, Key_Layout current);

const char *name_of_key_layout(Key_Layout layout);
Key_Layout key_layout_of_name(const char *name);
