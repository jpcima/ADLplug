//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "utility/simple_fifo.h"
#include <wopl/wopl_file.h>
#include <cstdint>

enum class Bank_Mode {
    Melodic, Percussive,
};

struct Message_Header {
    unsigned tag;
    unsigned size;
};

struct Buffered_Message {
    Message_Header *header = nullptr;
    uint8_t *data = nullptr;
    unsigned offset = 0;
    explicit operator bool() const
        { return data; }
};

Buffered_Message read_message(Simple_Fifo &fifo) noexcept;
void finish_read_message(Simple_Fifo &fifo, const Buffered_Message &msg);

Buffered_Message write_message(Simple_Fifo &fifo, const Message_Header &hdr) noexcept;
void finish_write_message(Simple_Fifo &fifo, Buffered_Message &msg);

//------------------------------------------------------------------------------
enum class User_Message {
    Midi,  // midi event
};

namespace Messages {
namespace User {

}  // namespace User
}  // namespace Messages

//------------------------------------------------------------------------------
enum class Fx_Message {
    Instrument,  // notifies an instrument when changed
};

namespace Messages {
namespace Fx {

struct Instrument
{
    uint16_t bank;
    uint8_t program;
    Bank_Mode mode;
    WOPLInstrument instrument;
};

}  // namespace Fx
}  // namespace Messages
