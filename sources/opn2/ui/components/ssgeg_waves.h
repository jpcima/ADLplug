//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "ui/components/waves.h"

class SSGEG_Waves : public Waves
{
public:
    unsigned wave_count() const override { return 8; }
    double compute_wave(unsigned wave, double phase) const override;

private:
    unsigned num_periods_ = 4;
};
