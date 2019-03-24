//          Copyright Jean Pierre Cimalando 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "plugin_state.h"
#include "parameter_block.h"
#include "adl/chip_settings.h"
#include "adl/player.h"

Chip_Settings get_player_chip_settings(const Player &pl)
{
    Chip_Settings cs;
    cs.emulator = pl.emulator();
    cs.chip_count = pl.num_chips();
#if defined(ADLPLUG_OPL3)
    cs.fourop_count = pl.num_4ops();
#elif defined(ADLPLUG_OPN2)
    cs.chip_type = pl.chip_type();
#endif
    return cs;
}

Instrument_Global_Parameters get_player_global_parameters(const Player &pl)
{
    Instrument_Global_Parameters gp;
    gp.volume_model = pl.volume_model() - 1;
#if defined(ADLPLUG_OPL3)
    gp.deep_tremolo = pl.deep_tremolo();
    gp.deep_vibrato = pl.deep_vibrato();
#elif defined(ADLPLUG_OPN2)
    gp.lfo_enable = pl.lfo_enabled();
    gp.lfo_frequency = pl.lfo_frequency();
#endif
    return gp;
}

void set_player_chip_settings(Player &pl, const Chip_Settings &cs)
{
    pl.set_emulator(cs.emulator);
    pl.set_num_chips(cs.chip_count);
#if defined(ADLPLUG_OPL3)
    pl.set_num_4ops(cs.fourop_count);
#elif defined(ADLPLUG_OPN2)
    pl.set_chip_type(cs.chip_type);
#endif
}

void set_player_global_parameters(Player &pl, const Instrument_Global_Parameters &gp)
{
    pl.set_volume_model(gp.volume_model + 1);
#if defined(ADLPLUG_OPL3)
    pl.set_deep_tremolo(gp.deep_tremolo);
    pl.set_deep_vibrato(gp.deep_vibrato);
#elif defined(ADLPLUG_OPN2)
    pl.set_lfo_enabled(gp.lfo_enable);
    pl.set_lfo_frequency(gp.lfo_frequency);
#endif
}
