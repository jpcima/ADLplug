//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "adl/instrument.h"
#include <wopl/wopl_file.h>
#include <adlmidi.h>

template <class Dst, class Src>
static inline void convert_WOPLish_instruments(Dst &dst, const Src &src)
{
#define COPY(x) dst.x = src.x;

    COPY(note_offset1);
    COPY(note_offset2);
    COPY(midi_velocity_offset);
    COPY(second_voice_detune);
    COPY(percussion_key_number);
    COPY(inst_flags);
    COPY(fb_conn1_C0);
    COPY(fb_conn2_C0);
    for (unsigned op = 0; op < 4; ++op) {
        COPY(operators[op].avekf_20);
        COPY(operators[op].ksl_l_40);
        COPY(operators[op].atdec_60);
        COPY(operators[op].susrel_80);
        COPY(operators[op].waveform_E0);
    }
    COPY(delay_on_ms);
    COPY(delay_off_ms);

#undef COPY
}

void convert_ADLI_from_WOPI(
    ADL_Instrument &dst, const WOPLInstrument &src)
{
    dst = ADL_Instrument{};
    dst.version = ADLMIDI_InstrumentVersion;
    convert_WOPLish_instruments(dst, src);
}

void convert_WOPI_from_ADLI(
    WOPLInstrument &dst, const ADL_Instrument &src)
{
    dst = WOPLInstrument{};
    convert_WOPLish_instruments(dst, src);
}
