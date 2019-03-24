//          Copyright Jean Pierre Cimalando 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

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
