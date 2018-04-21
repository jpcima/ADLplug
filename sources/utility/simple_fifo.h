//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Simple_Fifo
{
public:
    explicit Simple_Fifo(unsigned capacity);

    const uint8_t *read(unsigned length, unsigned &offset) const noexcept;
    bool read_padding(unsigned &offset) noexcept;
    void finish_read(unsigned length) noexcept
        { return fifo_.finishedRead(length); }

    uint8_t *write(unsigned length, unsigned &offset) noexcept;
    bool write_padding(unsigned &offset) noexcept;
    void finish_write(unsigned length) noexcept;

    unsigned get_free_space() const noexcept
        { return fifo_.getFreeSpace(); }
    unsigned get_num_ready() const noexcept
        { return fifo_.getNumReady(); }

private:
    AbstractFifo fifo_;
    std::unique_ptr<uint8_t[]> buffer_;

    static unsigned pad_offset(unsigned offset)
    {
        unsigned n = offset % alignof(max_align_t);
        return offset + (n ? (alignof(max_align_t) - n) : 0);
    }
};
