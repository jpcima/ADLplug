//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <math.h>

struct Vu_Monitor
{
    double p_ = 0;
    double mem_ = 0;
    void release(double t); // t = fs * release time
    double process(double x);
};

inline void Vu_Monitor::release(double t)
{
    p_ = exp(-1.0 / t);
}

inline double Vu_Monitor::process(double x)
{
    double y;
    double ax = fabs(x);
    double p = p_;
    if (ax > mem_)
        y = ax;
    else
        y = p * mem_ + (1.0 - p) * ax;
    mem_ = y;
    return y;
}
