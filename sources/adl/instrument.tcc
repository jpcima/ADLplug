//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "instrument.h"

template <unsigned shift, unsigned size, class T_result, class T_field>
inline T_result Instrument::get_bits(T_field x) noexcept
{
    return (x >> shift) & ((1u << size) - 1);
}

template <unsigned shift, unsigned size, class T_value, class T_field>
inline void Instrument::set_bits(T_field &x, T_value v) noexcept
{
    constexpr unsigned mask = (1u << size) - 1;
    x = (x & ~(T_field)(mask << shift)) | ((v & mask) << shift);
}

template <unsigned shift, unsigned size, class T_result, class T_field>
inline T_result Instrument::get_bits_inverted(T_field x) noexcept
{
    constexpr T_result max = ((1u << size) - 1);
    return max - get_bits<shift, size, T_result, T_field>(x);
}

template <unsigned shift, unsigned size, class T_value, class T_field>
inline void Instrument::set_bits_inverted(T_field &x, T_value v) noexcept
{
    constexpr T_value max = ((1u << size) - 1);
    set_bits<shift, size, T_value, T_field>(x, max - v);
}
