//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <adlmidi.h>
#include <stdint.h>
struct WOPLInstrument;
class Generic_Player;

struct Instrument : ADL_Instrument
{
    Instrument() noexcept
        : ADL_Instrument{} { inst_flags = ADLMIDI_Ins_IsBlank; }

    static Instrument from_adlmidi(const ADL_Instrument &o) noexcept;
    static Instrument from_wopl(const WOPLInstrument &o) noexcept;

    bool blank() const noexcept
        { return inst_flags & ADLMIDI_Ins_IsBlank; }

#if 0
    const char *name() const noexcept
        { return name_; }
    void name(const char *name) noexcept;

    static constexpr unsigned name_size = 32;
    char name_[name_size + 1] = {};
#endif
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
