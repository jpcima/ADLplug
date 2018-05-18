//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "adl/instrument.h"
#include "utility/simple_fifo.h"
#include <thread>
#include <cstdint>

enum class User_Message;
enum class Fx_Message;

struct Message_Header {
    unsigned tag {};
    unsigned size = 0;
    Message_Header(User_Message tag, unsigned size)
        : tag((unsigned)tag), size(size) {}
    Message_Header(Fx_Message tag, unsigned size)
        : tag((unsigned)tag), size(size) {}
};

struct Buffered_Message {
    Message_Header *header = nullptr;
    uint8_t *data = nullptr;
    unsigned offset = 0;
    explicit operator bool() const
        { return data; }
};

Buffered_Message read_message(Simple_Fifo &fifo) noexcept;
void finish_read_message(Simple_Fifo &fifo, const Buffered_Message &msg) noexcept;

Buffered_Message write_message(Simple_Fifo &fifo, const Message_Header &hdr) noexcept;
void finish_write_message(Simple_Fifo &fifo, Buffered_Message &msg) noexcept;

template <class R, class P>
Buffered_Message write_message_retrying(
    Simple_Fifo &fifo, const Message_Header &hdr, std::chrono::duration<R, P> delay)
{
    Buffered_Message msg;
    while (!(msg = write_message(fifo, hdr)))
        std::this_thread::sleep_for(delay);
    return msg;
}

//------------------------------------------------------------------------------
enum class User_Message {
    Midi,  // midi event
    LoadInstrument,  // edits an instrument
};

namespace Messages {
namespace User {

struct LoadInstrument
{
    Bank_Id bank;
    uint8_t program = 0;
    Instrument instrument;
};

}  // namespace User
}  // namespace Messages

//------------------------------------------------------------------------------
enum class Fx_Message {
    NotifyInstrument,  // notifies an instrument when changed
};

namespace Messages {
namespace Fx {

struct NotifyInstrument {
    Bank_Id bank;
    uint8_t program = 0;
    Instrument instrument;
};

}  // namespace Fx
}  // namespace Messages
