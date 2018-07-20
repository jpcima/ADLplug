//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "adl/instrument.h"
#include "utility/simple_fifo.h"
#include "definitions.h"
#include <thread>
#include <bitset>
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
    RequestBankSlots,  // requests the layout of banks and instruments
    RequestFullBankState,  // requests all the managed bank state
    ClearBanks,  // deletes all the managed banks
    LoadInstrument,  // edits an instrument
    SelectProgram,  // changes selected program number
};

namespace Messages {
namespace User {

struct RequestBankSlots {
};

struct RequestFullBankState {
};

struct ClearBanks {
    bool notify_back = false;
};

struct LoadInstrument
{
    Bank_Id bank;
    uint8_t program = 0;
    Instrument instrument;
    bool notify_back = false;
};

struct SelectProgram
{
    Bank_Id bank;
    uint8_t program = 0;
};

}  // namespace User
}  // namespace Messages

//------------------------------------------------------------------------------
enum class Fx_Message {
    NotifyBankSlots,  // notifies the layout of banks and instruments
    NotifyInstrument,  // notifies an instrument when changed
};

namespace Messages {
namespace Fx {

struct NotifyBankSlots {
    struct Entry {
        Bank_Id bank;
        std::bitset<128> ins_mask;
    };
    unsigned count;
    Entry entry[bank_reserve_size];
};

struct NotifyInstrument {
    Bank_Id bank;
    uint8_t program = 0;
    Instrument instrument;
};

}  // namespace Fx
}  // namespace Messages
