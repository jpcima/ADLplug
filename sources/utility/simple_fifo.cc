//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "utility/simple_fifo.h"
#include <cstring>
#include <cassert>

Simple_Fifo::Simple_Fifo(unsigned capacity)
    : fifo_(capacity),
      buffer_(new uint8_t[capacity * 2]())
{
}

const uint8_t *Simple_Fifo::read(unsigned length, unsigned &offset) const noexcept
{
    const AbstractFifo &fifo = fifo_;
    unsigned i1, n1, i2, n2, off = offset;
    fifo.prepareToRead(off + length, (int &)i1, (int &)n1, (int &)i2, (int &)n2);
    if (n1 + n2 != off + length)
        return nullptr;
    offset = off + length;
    return &buffer_[off + i1];
}

bool Simple_Fifo::read_padding(unsigned &offset) noexcept
{
    unsigned off = pad_offset(offset);
    if (off > get_num_ready())
        return false;
    offset = off;
    return true;
}

uint8_t *Simple_Fifo::write(unsigned length, unsigned &offset) noexcept
{
    const AbstractFifo &fifo = fifo_;
    unsigned i1, n1, i2, n2, off = offset;
    fifo.prepareToWrite(off + length, (int &)i1, (int &)n1, (int &)i2, (int &)n2);
    if (n1 + n2 != off + length)
        return nullptr;
    offset = off + length;
    return &buffer_[off + i1];
}

bool Simple_Fifo::write_padding(unsigned &offset) noexcept
{
    unsigned off = pad_offset(offset);
    if (off > get_free_space())
        return false;
    offset = off;
    return true;
}

void Simple_Fifo::finish_write(unsigned length) noexcept
{
    AbstractFifo &fifo = fifo_;
    unsigned capacity = fifo.getTotalSize();
    unsigned i1, n1, i2, n2;
    fifo.prepareToWrite(length, (int &)i1, (int &)n1, (int &)i2, (int &)n2);
    assert(length == n1 + n2);
    uint8_t *buffer = buffer_.get();
    for (unsigned i = 0; i < length; ++i) {
        unsigned src = i1 + i;
        unsigned dst = src + capacity;
        dst = (dst < 2 * capacity) ? dst : (dst - 2 * capacity);
        buffer[dst] = buffer[src];
    }
    fifo.finishedWrite(length);
}
