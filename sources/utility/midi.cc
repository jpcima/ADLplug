//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "utility/midi.h"

Midi_Input_Message Midi_Input_Source::midi_cb_for_buffer_iterator(void *cbdata)
{
    const uint8_t *data;
    int size, sample_pos;
    MidiBuffer::Iterator &it = *(MidiBuffer::Iterator *)cbdata;
    return it.getNextEvent(data, size, sample_pos) ?
        Midi_Input_Message(data, size, sample_pos) : Midi_Input_Message();
}
