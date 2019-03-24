//          Copyright Jean Pierre Cimalando 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>
class Player;
struct Parameter_Block;
struct Instrument;
struct Chip_Settings;
struct Instrument_Global_Parameters;
namespace juce { class XmlElement; }

enum State_Change_Bit
{
    Cb_ChipSettings,
    Cb_GlobalParameters,
    Cb_ActivePart,
    Cb_BankTitle,
    Cb_Instrument1,
    Cb_Instrument16 = Cb_Instrument1 + 15,
    Cb_Selection1,
    Cb_Selection16 = Cb_Selection1 + 15,
    Cb_Count,
};

Chip_Settings get_player_chip_settings(const Player &pl);
Instrument_Global_Parameters get_player_global_parameters(const Player &pl);
void set_player_chip_settings(Player &pl, const Chip_Settings &cs);
void set_player_global_parameters(Player &pl, const Instrument_Global_Parameters &gp);
