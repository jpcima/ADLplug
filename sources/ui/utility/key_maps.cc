//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "key_maps.h"
#include "configuration.h"

void set_key_layout(MidiKeyboardComponent &kb, Key_Layout layout, Configuration &conf)
{
    kb.clearKeyMappings();

    int note = 0;
    switch (layout) {
    case Key_Layout::Qwerty: default:
        conf.set_string("piano", "layout", "qwerty");
        break;
    case Key_Layout::Azerty:
        conf.set_string("piano", "layout", "azerty");
        break;
    }

    load_key_configuration(kb, conf);
    conf.save_default();
}

void load_key_configuration(MidiKeyboardComponent &kb, Configuration &conf)
{
    std::string layout = conf.get_string("piano", "layout", "qwerty");
    std::string keymap = conf.get_string("piano", ("keymap:" + layout).c_str(), "");
    if (keymap.empty())
        keymap = (layout == "azerty") ? "qzsedftgyhujkolpm" :
            /* qwerty */ "awsedftgyhujkolp;";

    int note = 0;
    kb.clearKeyMappings();
    for (unsigned char c : keymap)
        kb.setKeyPressForNote(KeyPress(c, 0, 0), note++);
}

void build_key_layout_menu(PopupMenu &menu)
{
    menu.addItem((int)Key_Layout::Qwerty + 1, "Use QWERTY keys");
    menu.addItem((int)Key_Layout::Azerty + 1, "Use AZERTY keys");
}
