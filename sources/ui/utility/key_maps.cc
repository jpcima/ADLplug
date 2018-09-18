//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "key_maps.h"

void install_key_layout(MidiKeyboardComponent &kb, Key_Layout layout)
{
    kb.clearKeyMappings();

    int note = 0;
    switch (layout) {
    case Key_Layout::Qwerty: default:
        for (unsigned char c : "awsedftgyhujkolp;")
            kb.setKeyPressForNote(KeyPress(c, 0, 0), note++);
        break;
    case Key_Layout::Azerty:
        for (unsigned char c : "qzsedftgyhujkolpm")
            kb.setKeyPressForNote(KeyPress(c, 0, 0), note++);
        break;
    }
}

void build_key_layout_menu(PopupMenu &menu)
{
    menu.addItem((int)Key_Layout::Qwerty + 1, "Use QWERTY keys");
    menu.addItem((int)Key_Layout::Azerty + 1, "Use AZERTY keys");
}
