//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"

class Vu_Meter : public Component
{
public:
    Vu_Meter();
    explicit Vu_Meter(const String &name);

    double value() const
        { return value_; }
    void set_value(double value);

    bool logarithmic() const
        { return logarithmic_; }
    void set_logarithmic(bool logarithmic);

    void set_hue(double start, double range);

protected:
    void paint(Graphics &g) override;

private:
    double value_ = 1.0;
    bool logarithmic_ = false;
    double hue_start_ = 0.0;
    double hue_range_ = 0.0;
};
