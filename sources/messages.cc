//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "messages.h"

Buffered_Message read_message(Simple_Fifo &fifo) noexcept
{
    Buffered_Message msg;
    msg.header = (Message_Header *)fifo.read(sizeof(Message_Header), msg.offset);
    if (!msg.header || !fifo.read_padding(msg.offset))
        return Buffered_Message{};
    msg.data = (uint8_t *)fifo.read(msg.header->size, msg.offset);
    if (!msg.data || !fifo.read_padding(msg.offset))
        return Buffered_Message{};
    return msg;
}

void finish_read_message(Simple_Fifo &fifo, const Buffered_Message &msg)
{
    fifo.finish_read(msg.offset);
}

Buffered_Message write_message(Simple_Fifo &fifo, const Message_Header &hdr) noexcept
{
    Buffered_Message msg;
    msg.header = (Message_Header *)fifo.write(sizeof(Message_Header), msg.offset);
    if (!msg.header || !fifo.write_padding(msg.offset))
        return Buffered_Message{};
    *msg.header = hdr;
    msg.data = fifo.write(msg.header->size, msg.offset);
    if (!msg.data || !fifo.write_padding(msg.offset))
        return Buffered_Message{};
    return msg;
}

void finish_write_message(Simple_Fifo &fifo, Buffered_Message &msg)
{
    fifo.finish_write(msg.offset);
}
