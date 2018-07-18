//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <adlmidi.h>
#include <stdint.h>
#include <stdio.h>
struct WOPLInstrument;
class Generic_Player;

struct Instrument : ADL_Instrument
{
    Instrument() noexcept
        : ADL_Instrument{} { inst_flags = ADLMIDI_Ins_IsBlank; }

    static Instrument from_adlmidi(const ADL_Instrument &o) noexcept;
    static Instrument from_wopl(const WOPLInstrument &o) noexcept;

public:
#define PARAMETER(type, id, field, shift, size, opt)            \
    type id() const noexcept                                    \
        { return get_bits##opt<shift, size, type>(field); }     \
    void id(type value) noexcept                                \
        { set_bits##opt<shift, size, type>(field, value); }

#define OP_PARAMETER(type, id, field, shift, size, opt)                 \
    type id(unsigned op) const noexcept                                 \
        { return get_bits##opt<shift, size, type>(operators[op].field); } \
    void id(unsigned op, type value) noexcept                           \
        { set_bits##opt<shift, size, type>(operators[op].field, value); }

    PARAMETER(bool, four_op, inst_flags, 0, 1,)
    PARAMETER(bool, pseudo_four_op, inst_flags, 1, 1,)
    PARAMETER(bool, blank, inst_flags, 2, 1,)
    PARAMETER(bool, con12, fb_conn1_C0, 0, 1,)
    PARAMETER(bool, con34, fb_conn2_C0, 0, 1,)
    PARAMETER(unsigned, fb12, fb_conn1_C0, 1, 3,)
    PARAMETER(unsigned, fb34, fb_conn2_C0, 1, 3,)
    OP_PARAMETER(unsigned, attack, atdec_60, 4, 4,)
    OP_PARAMETER(unsigned, decay, atdec_60, 0, 4,)
    OP_PARAMETER(unsigned, sustain, susrel_80, 4, 4, _inverted)
    OP_PARAMETER(unsigned, release, susrel_80, 0, 4,)
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

    void describe(FILE *out) const noexcept;
    void describe_operator(unsigned op, FILE *out, const char *indent = "") const noexcept;

    bool equal_instrument(const ADL_Instrument &o) const noexcept;

#if 0
    const char *name() const noexcept
        { return name_; }
    void name(const char *name) noexcept;

    static constexpr unsigned name_size = 32;
    char name_[name_size + 1] = {};
#endif

private:
    template <unsigned shift, unsigned size, class T_result, class T_field>
    static T_result get_bits(T_field x) noexcept;
    template <unsigned shift, unsigned size, class T_value, class T_field>
    static void set_bits(T_field &x, T_value v) noexcept;
    template <unsigned shift, unsigned size, class T_result, class T_field>
    static T_result get_bits_inverted(T_field x) noexcept;
    template <unsigned shift, unsigned size, class T_value, class T_field>
    static void set_bits_inverted(T_field &x, T_value v) noexcept;
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

class Bank_Lookup_Cache
{
public:
    Bank_Ref *get(Generic_Player &pl, const Bank_Id &id, int flags) noexcept;
    Bank_Ref &ensure_get(Generic_Player &pl, const Bank_Id &id, int flags) noexcept;
    void clear() noexcept { last_bank_id_ = Bank_Id(); }
private:
    Bank_Id last_bank_id_;
    Bank_Ref last_bank_;
};

#include "instrument.tcc"
