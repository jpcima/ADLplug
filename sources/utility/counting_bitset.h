//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <bitset>

template <size_t N>
struct counting_bitset {
    constexpr counting_bitset();

    counting_bitset(const counting_bitset &) = default;
    counting_bitset &operator=(const counting_bitset &) = default;

    bool operator==(const counting_bitset &o) const;
    bool operator!=(const counting_bitset &o) const;

    bool test(size_t pos) const;

    bool all() const;
    bool any() const;
    bool none() const;

    size_t count() const;

    counting_bitset &set();
    counting_bitset &set(size_t pos, bool value = true);

    counting_bitset &reset();
    counting_bitset &reset(size_t pos);

    counting_bitset &flip();
    counting_bitset &flip(size_t pos);

    template <class CharT = char, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
    std::basic_string<CharT, Traits, Allocator> to_string(CharT zero = CharT('0'), CharT one = CharT('1')) const;

private:
    size_t count_ = 0;
    std::bitset<N> bits_;
};

#include "counting_bitset.tcc"
