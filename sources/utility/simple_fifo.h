//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Simple_Fifo
{
public:
    explicit Simple_Fifo(unsigned cap);

    bool discard(unsigned length);
    bool read(uint8_t *data, unsigned length, bool consume = true);
    bool write(const uint8_t *data, unsigned length);

    unsigned get_free_space() const;
    unsigned get_num_ready() const;

private:
    AbstractFifo fifo_;
    std::unique_ptr<uint8_t[]> buffer_;
};
