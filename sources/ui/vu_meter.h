//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class Vu_Meter : public Component
{
public:
    Vu_Meter();
    explicit Vu_Meter(const String &name);

    double value() const
        { return value_; }
    void set_value(double value);

protected:
    void paint(Graphics &g) override;

private:
    double value_ = 1.0;
};
