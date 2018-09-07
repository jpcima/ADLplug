//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ssgeg_waves.h"

double SSGEG_Waves::compute_wave(unsigned wave, double phase) const
{
    wave = wave & 7;

    const unsigned num_periods = num_periods_;

    if (num_periods == 0)
        return 0.0;

    phase = (phase < 0) ? 0 : phase;
    phase = (phase > 1) ? 1 : phase;

    phase *= num_periods;
    unsigned period = (unsigned)phase;
    period = (period < num_periods) ? period : (num_periods - 1);
    phase -= period;

    bool att = (wave & 4) != 0;
    bool alt = (wave & 2) != 0;
    bool hold = (wave & 1) != 0;

    if (period > 0 && hold)  // Hold
        return (att ^ alt) ? 1.0 : -1.0;

    int dir = att ? +1 : -1;
    dir = (alt && (period & 1) != 0) ? -dir : dir;

    double d = phase * 2 - 1;
    return (dir == +1) ? +d : -d;
}
