//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "instrument.h"
#include "player.h"
#include <adlmidi.h>
#include <cstring>
#include <cassert>

#define EACH_INS_FIELD(F)                                               \
    F(note_offset1) F(note_offset2)                                     \
    F(midi_velocity_offset) F(second_voice_detune) F(percussion_key_number) \
    F(inst_flags)                                                       \
    F(fb_conn1_C0) F(fb_conn2_C0)                                       \
    F(delay_on_ms) F(delay_off_ms)

#define EACH_OP_FIELD(F)                        \
    F(avekf_20) F(ksl_l_40) F(atdec_60) F(susrel_80) F(waveform_E0)

Instrument Instrument::from_adlmidi(const ADL_Instrument &o) noexcept
{
    Instrument ins;
    std::memcpy(static_cast<ADL_Instrument *>(&ins), &o, sizeof(ADL_Instrument));
    return ins;
}

Instrument Instrument::from_wopl(const WOPLInstrument &o) noexcept
{
    Instrument ins;
    ins.version = ADLMIDI_InstrumentVersion;

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

WOPLInstrument Instrument::to_wopl() const noexcept
{
    WOPLInstrument ins = {};

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

Instrument Instrument::from_sbi(const uint8_t *data, size_t length) noexcept
{
    Instrument ins;
    ins.version = ADLMIDI_InstrumentVersion;
    ins.blank(true);

    if (length < 4 + 32)
        return ins;

    const uint8_t *magic = data;
    data += 4;
    length -= 4;

    enum {
        SBI_Dos, SBI_Unix2OP, SBI_Unix4OP, SBI_Other
    } kind;

    size_t minsize;
    if (!memcmp(magic, "SBI\x1a", 4)) {
        kind = SBI_Dos;
        minsize = 11;
    }
    else if (!memcmp(magic, "2OP\x1a", 4)) {
        kind = SBI_Unix2OP;
        minsize = 11;
    }
    else if (!memcmp(magic, "4OP\x1a", 4)) {
        kind = SBI_Unix4OP;
        minsize = 22;
    }
    else if (!memcmp(magic, "SBI", 3)) {
        kind = SBI_Other;
        minsize = 11;
    }
    else
        return ins;

    const uint8_t *name_field = data;
    if (kind != SBI_Unix2OP && kind != SBI_Unix4OP)
        memcpy(ins.name, name_field, 32);
    else
        memcpy(ins.name, name_field, 30);
    data += 32;
    length -= 32;

    if (length < minsize) {
        ins.blank(true);
        return ins;
    }

    for (unsigned i = 0; i < 2; ++i) {
        ins.operators[i].avekf_20 = data[0 + !i];
        ins.operators[i].ksl_l_40 = data[2 + !i];
        ins.operators[i].atdec_60 = data[4 + !i];
        ins.operators[i].susrel_80 = data[6 + !i];
        ins.operators[i].waveform_E0 = data[8 + !i];
    }
    ins.fb_conn1_C0 = data[10];
    data += 11;
    length -= 11;

    switch (kind) {
    case SBI_Dos:
        if (length > 1)
            ins.note_offset1 = (int8_t)data[1];
        if (length > 2)
            ins.percussion_key_number = data[2];
        break;

    case SBI_Unix4OP:
        ins.four_op(true);
        for (unsigned i = 0; i < 2; ++i) {
            ins.operators[2 + i].avekf_20 = data[0 + !i];
            ins.operators[2 + i].ksl_l_40 = data[2 + !i];
            ins.operators[2 + i].atdec_60 = data[4 + !i];
            ins.operators[2 + i].susrel_80 = data[6 + !i];
            ins.operators[2 + i].waveform_E0 = data[8 + !i];
        }
        ins.fb_conn2_C0 = data[10];
        /* fall through */
    case SBI_Unix2OP:
        ins.percussion_key_number = name_field[31];
        break;

    default:
        break;
    }

    ins.blank(false);
    return ins;
}

void Instrument::describe(FILE *out) const noexcept
{
    fprintf(stderr,
            "Instrument\n"
            " - 4Op %u Ps4Op %u Blank %u\n"
            " - 1-2 Feedback %u Conn %u Tune %d\n"
            " - 3-4 Feedback %u Conn %u Tune %d\n"
            " - Velocity offset %d\n"
            " - Second voice fine tune %d\n"
            " - Percussion note %u\n",
            four_op(), pseudo_four_op(), blank(),
            fb12(), con12(), note_offset1,
            fb34(), con34(), note_offset2,
            midi_velocity_offset, second_voice_detune, percussion_key_number);
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
            "%s - ADSR %u %u %u %u\n"
            "%s - Level %u FMul %u KSL %u\n"
            "%s - Trem %u Vib %u Sus %u Env %u\n"
            "%s - Wave %u\n",
            indent, op, text,
            indent, attack(op), decay(op), sustain(op), release(op),
            indent, level(op), fmul(op), ksl(op),
            indent, trem(op), vib(op), sus(op), env(op),
            indent, wave(op));
}

bool Instrument::equal_instrument(const ADL_Instrument &o) const noexcept
{
    return !std::memcmp(static_cast<const ADL_Instrument *>(this), &o, sizeof(ADL_Instrument));
}

bool Instrument::equal_instrument_except_delays(const ADL_Instrument &o) const noexcept
{
    ADL_Instrument samedelay = o;
    samedelay.delay_on_ms = this->delay_on_ms;
    samedelay.delay_off_ms = this->delay_off_ms;
    return equal_instrument(samedelay);
}

void Midi_Bank::from_wopl(const WOPLFile &wopl, std::vector<Midi_Bank> &banks, Instrument_Global_Parameters &igp)
{
    banks.clear();

    unsigned nm = wopl.banks_count_melodic;
    unsigned np = wopl.banks_count_percussion;
    banks.resize(nm + np);

    for (unsigned i = 0; i < nm + np; ++i) {
        Midi_Bank &bank = banks[i];
        bool percussive = i >= nm;
        const WOPLBank &src = percussive ?
            wopl.banks_percussive[i - nm] : wopl.banks_melodic[i];
        bank.id = Bank_Id(src.bank_midi_msb, src.bank_midi_lsb, percussive);
        for (unsigned i = 0; i < 128; ++i)
            bank.ins[i] = Instrument::from_wopl(src.ins[i]);
        memcpy(bank.name, src.bank_name, 32);
    }

    igp.volume_model = wopl.volume_model;
    igp.deep_tremolo = wopl.opl_flags & WOPL_FLAG_DEEP_TREMOLO;
    igp.deep_vibrato = wopl.opl_flags & WOPL_FLAG_DEEP_VIBRATO;
}
