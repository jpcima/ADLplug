//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace Field_Bitops {

template <unsigned shift, unsigned size, class T_result, class T_field>
inline T_result get(T_field x) noexcept
{
    return (x >> shift) & ((1u << size) - 1);
}

template <unsigned shift, unsigned size, class T_value, class T_field>
inline void set(T_field &x, T_value v) noexcept
{
    constexpr unsigned mask = (1u << size) - 1;
    x = (x & ~(T_field)(mask << shift)) | ((v & mask) << shift);
}

template <unsigned shift, unsigned size, class T_result, class T_field>
inline T_result get_inverted(T_field x) noexcept
{
    constexpr T_result max = ((1u << size) - 1);
    return max - get<shift, size, T_result, T_field>(x);
}

template <unsigned shift, unsigned size, class T_value, class T_field>
inline void set_inverted(T_field &x, T_value v) noexcept
{
    constexpr T_value max = ((1u << size) - 1);
    set<shift, size, T_value, T_field>(x, max - v);
}

}  // namespace Field_Bitops
