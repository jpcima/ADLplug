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

#pragma once
struct Instrument;

namespace Measurer
{
    struct DurationInfo
    {
        uint64_t    peak_amplitude_time;
        double      peak_amplitude_value;
        double      quarter_amplitude_time;
        double      begin_amplitude;
        double      interval;
        double      keyoff_out_time;
        int64_t     ms_sound_kon;
        int64_t     ms_sound_koff;
        bool        nosound;
    };

    void ComputeDurations(const Instrument &in, DurationInfo &result);
};
