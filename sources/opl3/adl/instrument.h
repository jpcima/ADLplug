//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "utility/field_bitops.h"
#include <wopl/wopl_file.h>
#include <adlmidi.h>
#include <vector>
#include <stdint.h>
#include <stdio.h>
struct WOPLInstrument;
struct WOPLFile;
class Player;
namespace juce { class PropertySet; }

struct Instrument : ADL_Instrument
{
    Instrument() noexcept
        : ADL_Instrument{} { inst_flags = ADLMIDI_Ins_IsBlank; }

    enum {
        latest_version = ADLMIDI_InstrumentVersion
    };

    static Instrument from_adlmidi(const ADL_Instrument &o) noexcept;
    static Instrument from_wopl(const WOPLInstrument &o) noexcept;
    WOPLInstrument to_wopl() const noexcept;

    static Instrument from_sbi(const uint8_t *data, size_t length) noexcept;

    juce::PropertySet to_properties() const;
    static Instrument from_properties(const juce::PropertySet &set);

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

    PARAMETER(bool, four_op, inst_flags, 0, 1,)
    PARAMETER(bool, pseudo_four_op, inst_flags, 1, 1,)
    PARAMETER(bool, blank, inst_flags, 2, 1,)
    PARAMETER(bool, con12, fb_conn1_C0, 0, 1,)
    PARAMETER(bool, con34, fb_conn2_C0, 0, 1,)
    PARAMETER(unsigned, fb12, fb_conn1_C0, 1, 3,)
    PARAMETER(unsigned, fb34, fb_conn2_C0, 1, 3,)
    OP_PARAMETER(unsigned, attack, atdec_60, 4, 4, _inverted)
    OP_PARAMETER(unsigned, decay, atdec_60, 0, 4, _inverted)
    OP_PARAMETER(unsigned, sustain, susrel_80, 4, 4, _inverted)
    OP_PARAMETER(unsigned, release, susrel_80, 0, 4, _inverted)
    OP_PARAMETER(unsigned, level, ksl_l_40, 0, 6, _inverted)
    OP_PARAMETER(unsigned, ksl, ksl_l_40, 6, 2,)
    OP_PARAMETER(unsigned, fmul, avekf_20, 0, 4,)
    OP_PARAMETER(bool, trem, avekf_20, 7, 1,)
    OP_PARAMETER(bool, vib, avekf_20, 6, 1,)
    OP_PARAMETER(bool, sus, avekf_20, 5, 1,)
    OP_PARAMETER(bool, env, avekf_20, 4, 1,)
    OP_PARAMETER(unsigned, wave, waveform_E0, 0, 3,)

#undef PARAMETER
#undef OP_PARAMETER

    char name[32] = {};

    void describe(FILE *out) const noexcept;
    void describe_operator(unsigned op, FILE *out, const char *indent = "") const noexcept;

    bool equal_instrument(const ADL_Instrument &o) const noexcept;
    bool equal_instrument_except_delays(const ADL_Instrument &o) const noexcept;
};

struct Instrument_Global_Parameters
{
    unsigned volume_model = 0;
    bool deep_tremolo = false;
    bool deep_vibrato = false;

    juce::PropertySet to_properties() const;
    static Instrument_Global_Parameters from_properties(const juce::PropertySet &set);
};

struct Bank_Ref : ADL_Bank
{
    constexpr Bank_Ref() noexcept
        : ADL_Bank{} {}
};

struct Bank_Id : ADL_BankId
{
    constexpr Bank_Id() noexcept
        : ADL_BankId{false, 0xff, 0xff} {}
    constexpr Bank_Id(uint8_t msb, uint8_t lsb, bool percussive) noexcept
        : ADL_BankId{percussive, msb, lsb} {}
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

    static void from_wopl(const WOPLFile &wopl, std::vector<Midi_Bank> &banks, Instrument_Global_Parameters &igp);
};
