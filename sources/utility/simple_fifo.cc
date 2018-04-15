//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "utility/simple_fifo.h"
#include <cstring>

Simple_Fifo::Simple_Fifo(unsigned cap)
    : fifo_(cap),
      buffer_(new uint8_t[cap])
{
}

bool Simple_Fifo::discard(unsigned length)
{
    AbstractFifo &fifo = fifo_;
    int i1, n1, i2, n2;
    fifo.prepareToRead(length, i1, n1, i2, n2);
    if (n1 + n2 < length)
        return false;
    fifo.finishedRead(length);
    return true;
}

bool Simple_Fifo::read(uint8_t *data, unsigned length, bool consume)
{
    AbstractFifo &fifo = fifo_;
    int i1, n1, i2, n2;
    fifo.prepareToRead(length, i1, n1, i2, n2);
    if (n1 + n2 < length)
        return false;
    const uint8_t *buffer = buffer_.get();
    memcpy(data, buffer + i1, n1);
    memcpy(data + n1, buffer + i2, n2);
    if (consume)
        fifo.finishedRead(length);
    return true;
}

bool Simple_Fifo::write(const uint8_t *data, unsigned length)
{
    AbstractFifo &fifo = fifo_;
    int i1, n1, i2, n2;
    fifo.prepareToWrite(length, i1, n1, i2, n2);
    if (n1 + n2 < length)
        return false;
    uint8_t *buffer = buffer_.get();
    memcpy(buffer + i1, data, n1);
    memcpy(buffer + i2, data + n1, n2);
    fifo.finishedWrite(length);
    return true;
}

unsigned Simple_Fifo::get_free_space() const
{
    return fifo_.getFreeSpace();
}

unsigned Simple_Fifo::get_num_ready() const
{
    return fifo_.getNumReady();
}
