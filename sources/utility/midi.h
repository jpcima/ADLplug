//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

struct Midi_Input_Message
{
    const uint8_t *data = nullptr;
    unsigned size = 0;

    Midi_Input_Message()
        {}
    Midi_Input_Message(const uint8_t *data, unsigned size)
        : data(data), size(size) {}

    explicit operator bool() const
        { return data; }
};

class Midi_Input_Source {
public:
    typedef Midi_Input_Message (callback_function)(void *);

    explicit Midi_Input_Source(callback_function *cb, void *cbdata = nullptr)
        : cb_(cb), cbdata_(cbdata) {}
    explicit Midi_Input_Source(MidiBuffer::Iterator &iterator)
        : cb_(&midi_cb_for_buffer_iterator), cbdata_(&iterator) {}

    Midi_Input_Message get_next_event()
        { return cb_(cbdata_); }

private:
    callback_function *cb_ = nullptr;
    void *cbdata_ = nullptr;
    static Midi_Input_Message midi_cb_for_buffer_iterator(void *cbdata);
};
