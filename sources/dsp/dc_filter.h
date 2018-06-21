//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <math.h>

struct Dc_Filter {
    void cutoff(double f);
    double process(double in);
    double b0_ = 0;
    double p_ = 0;
    double last_in_ = 0;
    double last_out_ = 0;
};

inline void Dc_Filter::cutoff(double f)
{
    double wn = M_PI * f;
    double b0 = b0_ = 1.0 / (1.0 + wn);
    p_ = (1.0 - wn) * b0;
}

inline double Dc_Filter::process(double in)
{
    in *= b0_;
    double out = (in - last_in_) + p_ * last_out_;
    last_in_ = in;
    last_out_ = out;
    return out;
}
