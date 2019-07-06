/*
 * OPL Bank Editor by Wohlstand, a free tool for music bank editing
 * Copyright (c) 2016-2018 Vitaly Novichkov <admin@wohlnet.ru>
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
#define M_PI 3.14159265358979323846
#endif

//Measurer needs an emulator
#include "chips/opl_chip_base.h"
#include "chips/dosbox_opl3.h"

typedef ADL::DosBoxOPL3 DefaultOPL3;

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

static const unsigned g_outputRate = 49716;

struct TinySynth
{
    ADL::OPLChipBase *m_chip;
    unsigned m_notesNum;
    int m_notenum;
    int8_t m_fineTune;
    int16_t m_noteOffsets[2];
    unsigned m_x[2];

    explicit TinySynth(ADL::OPLChipBase &chip)
        : m_chip(&chip)
    {
        resetChip();
    }

    void resetChip()
    {
        static const short initdata[] =
        {
            0x004, 96, 0x004, 128,      // Pulse timer
            0x105, 0, 0x105, 1, 0x105, 0, // Pulse OPL3 enable, leave disabled
            0x001, 32, 0x0BD, 0         // Enable wave & melodic
        };

        m_chip->setRate(g_outputRate);

        for(unsigned a = 0; a < sizeof(initdata) / sizeof(*initdata); a += 2)
            m_chip->writeReg((uint16_t)initdata[a], (uint8_t)initdata[a + 1]);
    }

    void setInstrument(const Instrument &in)
    {
        uint8_t rawData[2][11];

        std::memset(m_x, 0, sizeof(m_x));
        m_notenum = in.percussion_key_number >= 128 ? (in.percussion_key_number - 128) : in.percussion_key_number;
        if(m_notenum == 0)
            m_notenum = 25;
        m_notesNum = in.four_op() ? 2 : 1;
        m_fineTune = 0;
        m_noteOffsets[0] = in.note_offset1;
        m_noteOffsets[1] = in.note_offset2;
        if(in.four_op() && in.pseudo_four_op())
            m_fineTune = in.second_voice_detune;
        if((m_notesNum == 2) && !in.pseudo_four_op())
        {
            m_chip->writeReg(0x105, 1);
            m_chip->writeReg(0x104, 0xFF);
        }

        for (unsigned op = 0; op < 4; ++op) {
            uint8_t *data = rawData[(op < 2) ? 0 : 1];
            data += (op & 1) ? 0 : 1;
            data[0] = in.operators[op].avekf_20 & 0x3F; //For clearer measurement, disable tremolo and vibrato
            data[2] = in.operators[op].atdec_60;
            data[4] = in.operators[op].susrel_80;
            data[6] = in.operators[op].waveform_E0;
            data[8] = in.operators[op].ksl_l_40;
        }
        rawData[0][10] = in.fb_conn1_C0;
        rawData[1][10] = in.fb_conn2_C0;

        for(unsigned n = 0; n < m_notesNum; ++n)
        {
            static const unsigned char patchdata[11] =
            {0x20, 0x23, 0x60, 0x63, 0x80, 0x83, 0xE0, 0xE3, 0x40, 0x43, 0xC0};
            for(unsigned a = 0; a < 10; ++a)
                m_chip->writeReg(patchdata[a] + n * 8, rawData[n][a]);
            m_chip->writeReg(patchdata[10] + n * 8, rawData[n][10] | 0x30);
        }
    }

    void noteOn()
    {
        std::memset(m_x, 0, sizeof(m_x));
        for(unsigned n = 0; n < m_notesNum; ++n)
        {
            double hertz = 172.00093 * std::exp(0.057762265 * (m_notenum + m_noteOffsets[n]));
            if(hertz > 131071)
            {
                std::fprintf(stderr, "MEASURER WARNING: Why does note %d + note-offset %d produce hertz %g?          \n",
                             m_notenum, m_noteOffsets[n], hertz);
                hertz = 131071;
            }
            m_x[n] = 0x2000;
            while(hertz >= 1023.5)
            {
                hertz /= 2.0;    // Calculate octave
                m_x[n] += 0x400;
            }
            m_x[n] += (unsigned int)(hertz + 0.5);

            // Keyon the note
            m_chip->writeReg(0xA0 + n * 3, m_x[n] & 0xFF);
            m_chip->writeReg(0xB0 + n * 3, m_x[n] >> 8);
        }
    }

    void noteOff()
    {
        // Keyoff the note
        for(unsigned n = 0; n < m_notesNum; ++n)
            m_chip->writeReg(0xB0 + n * 3, (m_x[n] >> 8) & 0xDF);
    }

    void generate(int16_t *output, size_t frames)
    {
        m_chip->generate(output, frames);
    }
};

namespace Measurer {

void ComputeDurations(const Instrument &in, DurationInfo &result)
{
    DefaultOPL3 chip;
    AudioHistory<double> audioHistory;

    const unsigned interval             = 150;
    const unsigned samples_per_interval = g_outputRate / interval;

    const double historyLength = 0.1;  // maximum duration to memorize (seconds)
    audioHistory.reset(std::ceil(historyLength * g_outputRate));

    std::unique_ptr<double[]> window;
    window.reset(new double[audioHistory.capacity()]);
    unsigned winsize = 0;

    TinySynth synth(chip);
    synth.setInstrument(in);
    synth.noteOn();

    /* For capturing */
    const unsigned max_silent = 6;
    const unsigned max_on  = 40;
    const unsigned max_off = 60;

    unsigned max_period_on = max_on * interval;
    unsigned max_period_off = max_off * interval;

    const double min_coefficient_on = 0.008;
    const double min_coefficient_off = 0.2;

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

    result.ms_sound_kon  = (unsigned long)(quarter_amplitude_time * 1000.0 / interval);
    result.ms_sound_koff = (unsigned long)(keyoff_out_time * 1000.0 / interval);
    result.nosound = (peak_amplitude_value < 0.5) || ((sound_min >= -1) && (sound_max <= 1));
}

}  // namespace Measurer
