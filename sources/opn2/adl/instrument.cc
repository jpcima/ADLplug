//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "instrument.h"
#include "player.h"
#include <opnmidi.h>
#include <cstring>
#include <cassert>

#define EACH_INS_FIELD(F)                               \
    F(note_offset)                                      \
    F(midi_velocity_offset) F(percussion_key_number)    \
    F(inst_flags)                                       \
    F(fbalg) F(lfosens)                                 \
    F(delay_on_ms) F(delay_off_ms)

#define EACH_OP_FIELD(F)                                                \
    F(dtfm_30) F(level_40) F(rsatk_50) F(amdecay1_60) F(decay2_70) F(susrel_80) F(ssgeg_90)

Instrument Instrument::from_adlmidi(const OPN2_Instrument &o) noexcept
{
    Instrument ins;
    std::memcpy(static_cast<OPN2_Instrument *>(&ins), &o, sizeof(OPN2_Instrument));
    return ins;
}

Instrument Instrument::from_wopl(const WOPNInstrument &o) noexcept
{
    Instrument ins;
    ins.version = OPNMIDI_InstrumentVersion;

    #define F(x) ins.x = o.x;
    EACH_INS_FIELD(F)
    #undef F

    for (unsigned op = 0; op < 4; ++op) {
        #define F(x) ins.operators[op].x = o.operators[op].x;
        EACH_OP_FIELD(F)
        #undef F
    }

    std::memcpy(ins.name, o.inst_name, 32);

    return ins;
}

WOPNInstrument Instrument::to_wopl() const noexcept
{
    WOPNInstrument ins = {};

    #define F(x) ins.x = this->x;
    EACH_INS_FIELD(F)
    #undef F

    for (unsigned op = 0; op < 4; ++op) {
        #define F(x) ins.operators[op].x = this->operators[op].x;
        EACH_OP_FIELD(F)
        #undef F
    }

    std::memcpy(ins.inst_name, name, 32);

    return ins;
}

void Instrument::describe(FILE *out) const noexcept
{
    fprintf(stderr,
            "Instrument\n"
            " - Blank %u\n"
            " - Feedback %u Algorithm %u Tune %d\n"
            " - AM sensitivity %u FM sensitivity %d\n"
            " - Velocity offset %d\n"
            " - Percussion note %u\n",
            blank(),
            feedback(), algorithm(), note_offset,
            ams(), fms(),
            midi_velocity_offset, percussion_key_number);
    for (unsigned op = 0; op < 4; ++op)
        describe_operator(op, out, "    ");
}

void Instrument::describe_operator(unsigned op, FILE *out, const char *indent) const noexcept
{
    fprintf(stderr,
            "%sOperator %u\n"
            "%s - ADSR %u %u,%u %u %u\n"
            "%s - AM %u Level %u Rate scale %u Detune %u FMul %u\n"
            "%s - SSG-EG Enable %u Wave %u\n",
            indent, op,
            indent, attack(op), decay2(op), decay1(op), sustain(op), release(op),
            indent, am(op), level(op), ratescale(op), detune(op), fmul(op),
            indent, ssgenable(op), ssgwave(op));
}

bool Instrument::equal_instrument(const OPN2_Instrument &o) const noexcept
{
    return !std::memcmp(static_cast<const OPN2_Instrument *>(this), &o, sizeof(OPN2_Instrument));
}

bool Instrument::equal_instrument_except_delays(const OPN2_Instrument &o) const noexcept
{
    OPN2_Instrument samedelay = o;
    samedelay.delay_on_ms = this->delay_on_ms;
    samedelay.delay_off_ms = this->delay_off_ms;
    return equal_instrument(samedelay);
}
