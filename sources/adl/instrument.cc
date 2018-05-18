//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "adl/instrument.h"
#include "adl/player.h"
#include <wopl/wopl_file.h>
#include <adlmidi.h>
#include <cassert>

Instrument::Instrument(const WOPLInstrument &o) noexcept
    : ADL_Instrument{}
{
    version = ADLMIDI_InstrumentVersion;
#define COPY(x) this->x = o.x;
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

Bank_Ref *Bank_Lookup_Cache::get(Generic_Player &pl, const Bank_Id &id, int flags) noexcept
{
    if (id == last_bank_id_)
        return &last_bank_;
    if (!pl.get_bank(id, flags, last_bank_))
        return nullptr;
    last_bank_id_ = id;
    return &last_bank_;
}

Bank_Ref &Bank_Lookup_Cache::ensure_get(Generic_Player &pl, const Bank_Id &id, int flags) noexcept
{
    Bank_Ref *bank = get(pl, id, flags);
    assert(bank);
    return *bank;
}
