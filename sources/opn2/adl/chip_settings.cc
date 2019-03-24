//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "chip_settings.h"
#include "player.h"
#include "resources.h"
#include <mutex>
#include <memory>

RESOURCE(Res, emu_mame);
RESOURCE(Res, emu_nuked);
RESOURCE(Res, emu_gens);
RESOURCE(Res, emu_neko);

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
    unsigned count = (unsigned)choices.size();
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
        if (name.size() >= 4 && !memcmp(name.data(), "mame", 4))
            default_index = i;
    }
    defaults->default_index = (default_index != ~0u) ? default_index : 0;

    //
    defaults->images.reset(new Image[count]);
    Image icon_mame = ImageFileFormat::loadFrom(Res::emu_mame.data, Res::emu_mame.size);
    Image icon_nuked = ImageFileFormat::loadFrom(Res::emu_nuked.data, Res::emu_nuked.size);
    Image icon_gens = ImageFileFormat::loadFrom(Res::emu_gens.data, Res::emu_gens.size);
    Image icon_neko = ImageFileFormat::loadFrom(Res::emu_neko.data, Res::emu_neko.size);
    for (unsigned i = 0; i < count; ++i) {
        const String &name = defaults->choices[i];
        String lowerName = name.toLowerCase();
        if (lowerName.startsWith("mame"))
            defaults->images[i] = icon_mame;
        else if (lowerName.startsWith("nuked"))
            defaults->images[i] = icon_nuked;
        else if (lowerName.startsWith("gens"))
            defaults->images[i] = icon_gens;
        else if (lowerName.startsWith("neko"))
            defaults->images[i] = icon_neko;
    }

    emulator_defaults_ = std::move(defaults_u);
    return *defaults;
}

PropertySet Chip_Settings::to_properties() const
{
    PropertySet set;
    set.setValue("emulator", (int)emulator);
    set.setValue("chip_count", (int)chip_count);
    set.setValue("chip_type", (int)chip_type);
    return set;
}

Chip_Settings Chip_Settings::from_properties(const PropertySet &set)
{
    Chip_Settings cs;
    cs.emulator = set.getIntValue("emulator");
    cs.chip_count = set.getIntValue("chip_count");
    cs.chip_type = set.getIntValue("chip_type");
    return cs;
}
