/*
 * OPN2 Bank Editor by Wohlstand, a free tool for music bank editing
 * Copyright (c) 2017-2018 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "measurer.h"
#include "../instrument.h"
#include <vector>
#include <cmath>
#include <memory>
#include <cstring>
#include <cstdio>
#include <limits>

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

//Measurer is always needs for emulator
#include "chips/opn_chip_base.h"
#include "chips/mame_opn2.h"

typedef OPN::MameOPN2 DefaultOPN2;

template <class T>
class AudioHistory
{
    std::unique_ptr<T[]> m_data;
    size_t m_index = 0;  // points to the next write slot
    size_t m_length = 0;
    size_t m_capacity = 0;

public:
    size_t size() const { return m_length; }
    size_t capacity() const { return m_capacity; }
    const T *data() const { return &m_data[m_index + m_capacity - m_length]; }

    void reset(size_t capacity)
    {
        m_data.reset(new T[2 * capacity]());
        m_index = 0;
        m_length = 0;
        m_capacity = capacity;
    }

    void clear()
    {
        m_length = 0;
    }

    void add(const T &item)
    {
        T *data = m_data.get();
        const size_t capacity = m_capacity;
        size_t index = m_index;
        data[index] = item;
        data[index + capacity] = item;
        m_index = (index + 1 != capacity) ? (index + 1) : 0;
        size_t length = m_length + 1;
        m_length = (length < capacity) ? length : capacity;
    }
};

static void HannWindow(double *w, unsigned n)
{
    for (unsigned i = 0; i < n; ++i)
        w[i] = 0.5 * (1.0 - std::cos(2 * M_PI * i / (n - 1)));
}

static double MeasureRMS(const double *signal, const double *window, unsigned length)
{
    double mean = 0;
#pragma omp simd reduction(+: mean)
    for(unsigned i = 0; i < length; ++i)
        mean += window[i] * signal[i];
    mean /= length;

    double rms = 0;
#pragma omp simd reduction(+: rms)
    for(unsigned i = 0; i < length; ++i)
    {
        double diff = window[i] * signal[i] - mean;
        rms += diff * diff;
    }
    rms = std::sqrt(rms / (length - 1));

    return rms;
}

static const unsigned g_outputRate = 53267;

struct TinySynth
{
    //! Context of the chip emulator
    OPN::OPNChipBase *m_chip;
    //! Count of playing notes
    unsigned m_notesNum;
    //! MIDI note to play
    int m_notenum;
    //! Centy detune
    int8_t  m_fineTune;
    //! Half-tone offset
    int16_t m_noteOffsets[2];

    //! Absolute channel
    uint32_t    m_c;
    //! Port of OPN2 chip
    uint8_t     m_port;
    //! Relative channel
    uint8_t     m_cc;

    void resetChip()
    {
        m_chip->setRate(g_outputRate, 7670454);

        m_chip->writeReg(0, 0x22, 0x00);   //LFO off
        m_chip->writeReg(0, 0x27, 0x0 );   //Channel 3 mode normal

        //Shut up all channels
        m_chip->writeReg(0, 0x28, 0x00 );   //Note Off 0 channel
        m_chip->writeReg(0, 0x28, 0x01 );   //Note Off 1 channel
        m_chip->writeReg(0, 0x28, 0x02 );   //Note Off 2 channel
        m_chip->writeReg(0, 0x28, 0x04 );   //Note Off 3 channel
        m_chip->writeReg(0, 0x28, 0x05 );   //Note Off 4 channel
        m_chip->writeReg(0, 0x28, 0x06 );   //Note Off 5 channel

        //Disable DAC
        m_chip->writeReg(0, 0x2B, 0x0 );   //DAC off
    }

    void setInstrument(const Instrument &in)
    {
        m_notenum = in.percussion_key_number >= 128 ? (in.percussion_key_number - 128) : in.percussion_key_number;
        if(m_notenum == 0)
            m_notenum = 25;
        m_notesNum = 1;
        m_fineTune = 0;
        m_noteOffsets[0] = in.note_offset;
        //m_noteOffsets[1] = in.note_offset2;

        m_c = 0;
        m_port = (m_c <= 2) ? 0 : 1;
        m_cc   = m_c % 3;

        for(uint8_t op = 0; op < 4; op++)
        {
            m_chip->writeReg(m_port, 0x30 + (op * 4) + m_cc, in.operators[op].dtfm_30);
            m_chip->writeReg(m_port, 0x40 + (op * 4) + m_cc, in.operators[op].level_40);
            m_chip->writeReg(m_port, 0x50 + (op * 4) + m_cc, in.operators[op].rsatk_50);
            m_chip->writeReg(m_port, 0x60 + (op * 4) + m_cc, in.operators[op].amdecay1_60);
            m_chip->writeReg(m_port, 0x70 + (op * 4) + m_cc, in.operators[op].decay2_70);
            m_chip->writeReg(m_port, 0x80 + (op * 4) + m_cc, in.operators[op].susrel_80);
            m_chip->writeReg(m_port, 0x90 + (op * 4) + m_cc, in.operators[op].ssgeg_90);
        }
        m_chip->writeReg(m_port, 0xB0 + m_cc, in.fbalg);
        m_chip->writeReg(m_port, 0xB4 + m_cc, 0xC0);//Disable LFO sensitivity for clear measure
    }

    void noteOn()
    {
        double hertz = 321.88557 * std::exp(0.057762265 * (m_notenum + m_noteOffsets[0]));
        uint16_t x2 = 0x0000;
        if(hertz < 0 || hertz > 262143)
        {
            std::fprintf(stderr, "MEASURER WARNING: Why does note %d + note-offset %d produce hertz %g?          \n",
                         m_notenum, m_noteOffsets[0], hertz);
            hertz = 262143;
        }

        while(hertz >= 2047.5)
        {
            hertz /= 2.0;    // Calculate octave
            x2 += 0x800;
        }
        x2 += static_cast<uint32_t>(hertz + 0.5);

        // Keyon the note
        m_chip->writeReg(m_port, 0xA4 + m_cc, (x2>>8) & 0xFF);//Set frequency and octave
        m_chip->writeReg(m_port, 0xA0 + m_cc,  x2 & 0xFF);

        m_chip->writeReg(0, 0x28, 0xF0 + uint8_t((m_c <= 2) ? m_c : m_c + 1));
    }

    void noteOff()
    {
        // Keyoff the note
        uint8_t cc = static_cast<uint8_t>(m_c % 6);
        m_chip->writeReg(0, 0x28, (m_c <= 2) ? cc : cc + 1);
    }

    void generate(int16_t *output, size_t frames)
    {
        m_chip->generate(output, frames);
    }
};

namespace Measurer {

void ComputeDurations(const Instrument &in, DurationInfo &result)
{
    DefaultOPN2 chip;
    AudioHistory<double> audioHistory;

    const unsigned interval             = 150;
    const unsigned samples_per_interval = g_outputRate / interval;

    const double historyLength = 0.1;  // maximum duration to memorize (seconds)
    audioHistory.reset(std::ceil(historyLength * g_outputRate));

    std::unique_ptr<double[]> window;
    window.reset(new double[audioHistory.capacity()]);
    unsigned winsize = 0;

    TinySynth synth;
    synth.m_chip = &chip;
    synth.resetChip();
    synth.setInstrument(in);
    synth.noteOn();

    /* For capturing */
    const unsigned max_silent = 6;
    const unsigned max_on  = 40;
    const unsigned max_off = 60;

    unsigned max_period_on = max_on * interval;
    unsigned max_period_off = max_off * interval;

    const double min_coefficient_on = 0.008;
    const double min_coefficient_off = 0.1;

    unsigned windows_passed_on = 0;
    unsigned windows_passed_off = 0;

    /* For Analyze the results */
    double begin_amplitude        = 0;
    double peak_amplitude_value   = 0;
    size_t peak_amplitude_time    = 0;
    size_t quarter_amplitude_time = max_period_on;
    bool   quarter_amplitude_time_found = false;
    size_t keyoff_out_time        = 0;
    bool   keyoff_out_time_found  = false;

    const size_t audioBufferLength = 256;
    const size_t audioBufferSize = 2 * audioBufferLength;
    int16_t audioBuffer[audioBufferSize];

    // For up to 40 seconds, measure mean amplitude.
    double highest_sofar = 0;
    short sound_min = 0, sound_max = 0;

    for(unsigned period = 0; period < max_period_on; ++period, ++windows_passed_on)
    {
        for(unsigned i = 0; i < samples_per_interval;)
        {
            size_t blocksize = samples_per_interval - i;
            blocksize = (blocksize < audioBufferLength) ? blocksize : audioBufferLength;
            synth.generate(audioBuffer, blocksize);
            for (unsigned j = 0; j < blocksize; ++j)
            {
                int16_t s = audioBuffer[2 * j];
                audioHistory.add(s);
                if(sound_min > s) sound_min = s;
                if(sound_max < s) sound_max = s;
            }
            i += blocksize;
        }

        if(winsize != audioHistory.size())
        {
            winsize = audioHistory.size();
            HannWindow(window.get(), winsize);
        }

        double rms = MeasureRMS(audioHistory.data(), window.get(), winsize);
        /* ======== Peak time detection ======== */
        if(period == 0)
        {
            begin_amplitude = rms;
            peak_amplitude_value = rms;
            peak_amplitude_time = 0;
        }
        else if(rms > peak_amplitude_value)
        {
            peak_amplitude_value = rms;
            peak_amplitude_time  = period;
            // In next step, update the quater amplitude time
            quarter_amplitude_time_found = false;
        }
        else if(!quarter_amplitude_time_found && (rms <= peak_amplitude_value * min_coefficient_on))
        {
            quarter_amplitude_time = period;
            quarter_amplitude_time_found = true;
        }
        /* ======== Peak time detection =END==== */
        if(rms > highest_sofar)
            highest_sofar = rms;

        if((period > max_silent * interval) &&
           ( (rms < highest_sofar * min_coefficient_on) || (sound_min >= -1 && sound_max <= 1) )
        )
            break;
    }

    if(!quarter_amplitude_time_found)
        quarter_amplitude_time = windows_passed_on;

    if(windows_passed_on >= max_period_on)
    {
        // Just Keyoff the note
        synth.noteOff();
    }
    else
    {
        // Reset the emulator and re-run the "ON" simulation until reaching the peak time
        synth.resetChip();
        synth.setInstrument(in);
        synth.noteOn();

        audioHistory.reset(std::ceil(historyLength * g_outputRate));
        for(unsigned period = 0;
            ((period < peak_amplitude_time) || (period == 0)) && (period < max_period_on);
            ++period)
        {
            for(unsigned i = 0; i < samples_per_interval;)
            {
                size_t blocksize = samples_per_interval - i;
                blocksize = (blocksize < audioBufferLength) ? blocksize : audioBufferLength;
                synth.generate(audioBuffer, blocksize);
                for (unsigned j = 0; j < blocksize; ++j)
                    audioHistory.add(audioBuffer[2 * j]);
                i += blocksize;
            }
        }
        synth.noteOff();
    }

    // Now, for up to 60 seconds, measure mean amplitude.
    for(unsigned period = 0; period < max_period_off; ++period, ++windows_passed_off)
    {
        for(unsigned i = 0; i < samples_per_interval;)
        {
            size_t blocksize = samples_per_interval - i;
            blocksize = (blocksize < 256) ? blocksize : 256;
            synth.generate(audioBuffer, blocksize);
            for (unsigned j = 0; j < blocksize; ++j)
            {
                int16_t s = audioBuffer[2 * j];
                audioHistory.add(s);
                if(sound_min > s) sound_min = s;
                if(sound_max < s) sound_max = s;
            }
            i += blocksize;
        }

        if(winsize != audioHistory.size())
        {
            winsize = audioHistory.size();
            HannWindow(window.get(), winsize);
        }

        double rms = MeasureRMS(audioHistory.data(), window.get(), winsize);
        /* ======== Find Key Off time ======== */
        if(!keyoff_out_time_found && (rms <= peak_amplitude_value * min_coefficient_off))
        {
            keyoff_out_time = period;
            keyoff_out_time_found = true;
        }
        /* ======== Find Key Off time ==END=== */
        if(rms < highest_sofar * min_coefficient_off)
            break;

        if((period > max_silent * interval) && (sound_min >= -1 && sound_max <= 1))
            break;
    }

    result.peak_amplitude_time = peak_amplitude_time;
    result.peak_amplitude_value = peak_amplitude_value;
    result.begin_amplitude = begin_amplitude;
    result.quarter_amplitude_time = (double)quarter_amplitude_time;
    result.keyoff_out_time = (double)keyoff_out_time;

    result.ms_sound_kon  = (int64_t)(quarter_amplitude_time * 1000.0 / interval);
    result.ms_sound_koff = (int64_t)(keyoff_out_time        * 1000.0 / interval);
    result.nosound = (peak_amplitude_value < 0.5) || ((sound_min >= -1) && (sound_max <= 1));
}

}  // namespace Measurer
