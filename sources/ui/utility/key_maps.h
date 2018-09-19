//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
class Configuration;

enum class Key_Layout {
    Qwerty, Azerty,
    Default = Qwerty,
};

void set_key_layout(MidiKeyboardComponent &kb, Key_Layout layout, Configuration &conf);
void load_key_configuration(MidiKeyboardComponent &kb, Configuration &conf);
void build_key_layout_menu(PopupMenu &menu);
