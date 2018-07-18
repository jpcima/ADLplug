//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "adl/instrument.h"
#include "adl/player.h"
#include <wopl/wopl_file.h>
#include <adlmidi.h>
#include <string.h>
#include <cassert>

Instrument Instrument::from_adlmidi(const ADL_Instrument &o) noexcept
{
    Instrument ins;
    memcpy(static_cast<ADL_Instrument *>(&ins), &o, sizeof(ADL_Instrument));
    return ins;
}

Instrument Instrument::from_wopl(const WOPLInstrument &o) noexcept
{
    Instrument ins;
    ins.version = ADLMIDI_InstrumentVersion;
#define COPY(x) ins.x = o.x;
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

#if 0
    ins.name(o.inst_name);
#endif

    return ins;
}

void Instrument::describe(FILE *out) const noexcept
{
    fprintf(stderr,
            "Instrument\n"
            "- 1-2 Feedback %u Conn %u\n"
            "- 3-4 Feedback %u Conn %u\n",
            fb12(), con12(),
            fb34(), con34());
    for (unsigned op = 0; op < 4; ++op)
        describe_operator(op, out, "    ");
}

void Instrument::describe_operator(unsigned op, FILE *out, const char *indent) const noexcept
{
    const char *text = nullptr;
    switch (op) {
    case WOPL_OP_MODULATOR1: text = "Modulator 1"; break;
    case WOPL_OP_CARRIER1: text = "Carrier 1"; break;
    case WOPL_OP_MODULATOR2: text = "Modulator 2"; break;
    case WOPL_OP_CARRIER2: text = "Carrier 2"; break;
    }
    fprintf(stderr,
            "%sOperator %u: %s\n"
            "%s- ADSR %u %u %u %u\n"
            "%s- Level %u FMul %u KSL %u\n"
            "%s- Trem %u Vib %u Sus %u Env %u\n"
            "%s- Wave %u\n",
            indent, op, text,
            indent, attack(op), decay(op), sustain(op), release(op),
            indent, level(op), fmul(op), ksl(op),
            indent, trem(op), vib(op), sus(op), env(op),
            indent, wave(op));
}

bool Instrument::equal_instrument(const ADL_Instrument &o) const noexcept
{
    return !memcmp(static_cast<const ADL_Instrument *>(this), &o, sizeof(ADL_Instrument));
}

#if 0
void Instrument::name(const char *name) noexcept
{
    memcpy(name_, name, strnlen(name, name_size));
}
#endif

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
