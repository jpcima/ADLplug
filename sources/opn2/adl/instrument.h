//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "utility/field_bitops.h"
#include <wopn/wopn_file.h>
#include <opnmidi.h>
#include <vector>
#include <stdint.h>
#include <stdio.h>
struct WOPNInstrument;
struct WOPNFile;
class Player;
namespace juce { class PropertySet; }

struct Instrument : OPN2_Instrument
{
    Instrument() noexcept
        : OPN2_Instrument{} { inst_flags = OPNMIDI_Ins_IsBlank; }

    enum {
        latest_version = OPNMIDI_InstrumentVersion
    };

    static Instrument from_adlmidi(const OPN2_Instrument &o) noexcept;
    static Instrument from_wopl(const WOPNInstrument &o) noexcept;
    WOPNInstrument to_wopl() const noexcept;

    void to_properties(juce::PropertySet &set, const char *key_prefix) const;
    static Instrument from_properties(const juce::PropertySet &set, const char *key_prefix);

public:
#define PARAMETER(type, id, field, shift, size, opt)                    \
    type id() const noexcept                                            \
        { return Field_Bitops::get##opt<shift, size, type>(field); }    \
    void id(type value) noexcept                                        \
        { Field_Bitops::set##opt<shift, size, type>(field, value); }

#define OP_PARAMETER(type, id, field, shift, size, opt)                 \
    type id(unsigned op) const noexcept                                 \
        { return Field_Bitops::get##opt<shift, size, type>(operators[op].field); } \
    void id(unsigned op, type value) noexcept                           \
        { Field_Bitops::set##opt<shift, size, type>(operators[op].field, value); }

    PARAMETER(bool, blank, inst_flags, 1, 1,)
    // PARAMETER(bool, pseudo_eight_op, inst_flags, 2, 1,)
    PARAMETER(unsigned, feedback, fbalg, 3, 3,)
    PARAMETER(unsigned, algorithm, fbalg, 0, 3,)
    PARAMETER(unsigned, ams, lfosens, 4, 2,)
    PARAMETER(unsigned, fms, lfosens, 0, 3,)
    OP_PARAMETER(unsigned, detune, dtfm_30, 4, 3,)
    OP_PARAMETER(unsigned, fmul, dtfm_30, 0, 4,)
    OP_PARAMETER(unsigned, level, level_40, 0, 7, _inverted)
    OP_PARAMETER(unsigned, ratescale, rsatk_50, 6, 2,)
    OP_PARAMETER(unsigned, attack, rsatk_50, 0, 5,)
    OP_PARAMETER(bool, am, amdecay1_60, 7, 1,)
    OP_PARAMETER(unsigned, decay1, amdecay1_60, 0, 5,)
    OP_PARAMETER(unsigned, decay2, decay2_70, 0, 5,)
    OP_PARAMETER(unsigned, sustain, susrel_80, 4, 4,)
    OP_PARAMETER(unsigned, release, susrel_80, 0, 4,)
    OP_PARAMETER(bool, ssgenable, ssgeg_90, 3, 1,)
    OP_PARAMETER(unsigned, ssgwave, ssgeg_90, 0, 3,)

#undef PARAMETER
#undef OP_PARAMETER

    char name[32] = {};

    void describe(FILE *out) const noexcept;
    void describe_operator(unsigned op, FILE *out, const char *indent = "") const noexcept;

    bool equal_instrument(const OPN2_Instrument &o) const noexcept;
    bool equal_instrument_except_delays(const OPN2_Instrument &o) const noexcept;
};

struct Instrument_Global_Parameters
{
    unsigned volume_model = 0;
    bool lfo_enable = false;
    unsigned lfo_frequency = 0;
};

struct Bank_Ref : OPN2_Bank
{
    constexpr Bank_Ref() noexcept
        : OPN2_Bank{} {}
};

struct Bank_Id : OPN2_BankId
{
    constexpr Bank_Id() noexcept
        : OPN2_BankId{false, 0xff, 0xff} {}
    constexpr Bank_Id(uint8_t msb, uint8_t lsb, bool percussive) noexcept
        : OPN2_BankId{percussive, msb, lsb} {}
    constexpr explicit operator bool() const noexcept
        { return msb < 0x7f; }
    constexpr bool operator==(const Bank_Id &o) const noexcept
        { return msb == o.msb && lsb == o.lsb && (bool)percussive == (bool)o.percussive; }
    constexpr bool operator!=(const Bank_Id &o) const noexcept
        { return !operator==(o); }
    uint32_t pseudo_id() const
        { return ((msb & 127) << 7) | (lsb & 127); }
    uint32_t to_integer() const
        { return ((msb & 127) << 8) | ((lsb & 127) << 1) | (percussive & 1); }
    static Bank_Id from_integer(uint32_t x)
        { return Bank_Id((x >> 8) & 127, (x >> 1) & 127, x & 1); }
};

inline bool operator<(Bank_Id a, Bank_Id b)
{
    return a.to_integer() < b.to_integer();
}

struct Midi_Bank
{
    Bank_Id id;
    Instrument ins[128];
    char name[32] = {};

    static void from_wopl(const WOPNFile &wopl, std::vector<Midi_Bank> &banks, Instrument_Global_Parameters &igp);
};
