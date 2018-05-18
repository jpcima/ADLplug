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
    Instrument()
        : ADL_Instrument{} { inst_flags = ADLMIDI_Ins_IsBlank; }
    Instrument(const ADL_Instrument &o)
        : ADL_Instrument(o) {}
    Instrument(const WOPLInstrument &o);
};

struct Bank_Ref : ADL_Bank
{
    constexpr Bank_Ref()
        : ADL_Bank{} {}
};

struct Bank_Id : ADL_BankId
{
    constexpr Bank_Id() noexcept
        : ADL_BankId{0xff} {}
    constexpr Bank_Id(uint8_t msb, uint8_t lsb, bool percussive) noexcept
        : ADL_BankId{msb, lsb, percussive} {}
    constexpr explicit operator bool() const
        { return msb < 0x7f; }
    constexpr bool operator==(const Bank_Id &o) const
        { return msb == o.msb && lsb == o.lsb && percussive == o.percussive; }
    constexpr bool operator!=(const Bank_Id &o) const
        { return !operator==(o); }
};

class Bank_Lookup_Cache
{
public:
    Bank_Ref *get(Generic_Player &pl, const Bank_Id &id, int flags);
    void clear() { last_bank_id_ = Bank_Id(); }
private:
    Bank_Id last_bank_id_;
    Bank_Ref last_bank_;
};
