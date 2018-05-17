//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

struct WOPLInstrument;
struct ADL_Instrument;

void convert_ADLI_from_WOPI(
    ADL_Instrument &dst, const WOPLInstrument &src);
void convert_WOPI_from_ADLI(
    WOPLInstrument &dst, const ADL_Instrument &src);
