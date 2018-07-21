//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "counting_bitset.h"

template <size_t N>
constexpr counting_bitset<N>::counting_bitset()
{
}

template <size_t N>
bool counting_bitset<N>::operator==(const counting_bitset &o) const
{
    return count_ == o.count_ && bits_ == o.bits_;
}

template <size_t N>
bool counting_bitset<N>::operator!=(const counting_bitset &o) const
{
    return !operator==(o);
}

template <size_t N>
bool counting_bitset<N>::test(size_t pos) const
{
    return bits_.test(pos);
}

template <size_t N>
bool counting_bitset<N>::all() const
{
    return count_ == N;
}

template <size_t N>
bool counting_bitset<N>::any() const
{
    return count_ > 0;
}

template <size_t N>
bool counting_bitset<N>::none() const
{
    return count_ == 0;
}

template <size_t N>
size_t counting_bitset<N>::count() const
{
    return count_;
}

template <size_t N>
auto counting_bitset<N>::set() -> counting_bitset &
{
    count_ = N;
    bits_.set();
    return *this;
}

template <size_t N>
auto counting_bitset<N>::set(size_t pos, bool value) -> counting_bitset &
{
    if (bits_.test(pos) != value) {
        count_ = (ptrdiff_t)count_ + (value ? +1 : -1);
        bits_.set(pos, value);
    }
    return *this;
}

template <size_t N>
auto counting_bitset<N>::reset() -> counting_bitset &
{
    count_ = 0;
    bits_.reset();
    return *this;
}

template <size_t N>
auto counting_bitset<N>::reset(size_t pos) -> counting_bitset &
{
    set(pos, false);
    return *this;
}

template <size_t N>
auto counting_bitset<N>::flip() -> counting_bitset &
{
    count_ = N - count_;
    bits_.flip();
    return *this;
}

template <size_t N>
auto counting_bitset<N>::flip(size_t pos) -> counting_bitset &
{
    bool value = !bits_.test(pos);
    count_ = (ptrdiff_t)count_ + (value ? +1 : -1);
    bits_.set(pos, value);
    return *this;
}

template <size_t N>
template<class CharT, class Traits, class Allocator>
std::basic_string<CharT, Traits, Allocator> counting_bitset<N>::to_string(CharT zero, CharT one) const
{
    return bits_.template to_string<CharT, Traits, Allocator>();
}
