//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "key_maps.h"
#include "configuration.h"
#include <algorithm>
#include <string.h>

const std::array<const char *, 3> key_layout_names {
    "qwerty",
    "qwertz",
    "azerty",
};
const std::array<const char32_t *, 3> key_layout_maps {
    U"zsxdcvgbhnjmq2w3er5t6y7ui9o0p",
    U"ysxdcvgbhnjmq2w3er5t6z7ui9o0p",
    U"wsxdcvgbhnj,aéz\"er(t-yèuiçoàp",
};

Key_Layout set_key_layout(Midi_Keyboard_Ex &kb, Key_Layout layout, Configuration &conf)
{
    kb.clearKeyMappings();

    const char *layout_name = name_of_key_layout(layout);
    conf.set_string("piano", "layout", layout_name);
    layout = load_key_configuration(kb, conf);
    conf.save_default();
    return layout;
}

Key_Layout load_key_configuration(Midi_Keyboard_Ex &kb, Configuration &conf)
{
    const char *layout_name = conf.get_string("piano", "layout", key_layout_names[0]);
    Key_Layout layout = key_layout_of_name(layout_name);
    layout_name = key_layout_names[(unsigned)layout];

    CharPointer_UTF32 keymap_unicode(
        (const juce_wchar *)key_layout_maps[(unsigned)layout]);

    String conf_keymap;
    if (const char *value = conf.get_string(
            "piano", (std::string("keymap:") + layout_name).c_str(), nullptr)) {
        conf_keymap = CharPointer_UTF8(value);
        keymap_unicode = conf_keymap.toUTF32();
    }

    int note = 0;
    kb.clearKeyMappings();

    for (CharPointer_UTF32 p = keymap_unicode; *p; ++p)
        kb.setKeyPressForNote(KeyPress(*p, 0, 0), note++);

    return layout;
}

void build_key_layout_menu(PopupMenu &menu, Key_Layout current)
{
    for (unsigned i = 0; i < key_layout_names.size(); ++i)
        menu.addItem(
            i + 1,
            "Use " + String(key_layout_names[i]).toUpperCase() + " keys",
            true, (Key_Layout)i == current);
}

const char *name_of_key_layout(Key_Layout layout)
{
    if ((unsigned)layout >= key_layout_names.size())
        layout = Key_Layout::Default;
    return key_layout_names[(unsigned)layout];
}

Key_Layout key_layout_of_name(const char *name)
{
    for (unsigned i = 0; i < key_layout_names.size(); ++i) {
        if (!strcmp(name, key_layout_names[i]))
            return (Key_Layout)i;
    }
    return Key_Layout::Default;
}
