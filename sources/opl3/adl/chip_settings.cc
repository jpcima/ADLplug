//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "chip_settings.h"
#include "player.h"
#include "resources.h"
#include <mutex>
#include <memory>

RESOURCE(Res, emu_dosbox);
RESOURCE(Res, emu_nuked);
RESOURCE(Res, emu_nuked2);

std::unique_ptr<Emulator_Defaults> emulator_defaults_;
std::mutex emulator_defaults_mutex_;

Emulator_Defaults &get_emulator_defaults()
{
    if (emulator_defaults_)
        return *emulator_defaults_;

    std::lock_guard<std::mutex> lock(emulator_defaults_mutex_);

    if (emulator_defaults_)
        return *emulator_defaults_;

    Emulator_Defaults *defaults = new Emulator_Defaults;
    std::unique_ptr<Emulator_Defaults> defaults_u(defaults);

    //
    std::vector<std::string> choices = Player::enumerate_emulators();
    unsigned count = choices.size();
    defaults->choices.ensureStorageAllocated(count);
    for (const std::string &choice : choices)
        defaults->choices.add(choice);

    //
    unsigned default_index = ~0u;
    for (unsigned i = 0; i < count && default_index == ~0u; ++i) {
        std::string name = choices[i];
        std::transform(name.begin(), name.end(), name.begin(),
                       [](unsigned char c) -> unsigned char
                           { return (c >= 'A' && c <= 'Z') ? (c - 'A' + 'a') : c; });
        if (name.size() >= 6 && !memcmp(name.data(), "dosbox", 6))
            default_index = i;
    }
    defaults->default_index = (default_index != ~0u) ? default_index : 0;

    //
    defaults->images.reset(new Image[count]);
    Image icon_dosbox = ImageFileFormat::loadFrom(Res::emu_dosbox.data, Res::emu_dosbox.size);
    Image icon_nuked = ImageFileFormat::loadFrom(Res::emu_nuked.data, Res::emu_nuked.size);
    Image icon_nuked2 = ImageFileFormat::loadFrom(Res::emu_nuked2.data, Res::emu_nuked2.size);
    unsigned nth_icon_nuked = 0;
    for (unsigned i = 0; i < count; ++i) {
        const String &name = defaults->choices[i];
        String lowerName = name.toLowerCase();
        if (lowerName.startsWith("dosbox"))
            defaults->images[i] = icon_dosbox;
        else if (lowerName.startsWith("nuked"))
            defaults->images[i] = (nth_icon_nuked++ == 0) ? icon_nuked : icon_nuked2;
    }

    emulator_defaults_ = std::move(defaults_u);
    return *defaults;
}
