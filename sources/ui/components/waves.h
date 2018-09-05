//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

class Waves {
public:
    virtual ~Waves() {}
    virtual unsigned wave_count() const = 0;
    virtual double compute_wave(unsigned wave, double phase) const = 0;
};
