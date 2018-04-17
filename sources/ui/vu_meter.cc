//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/vu_meter.h"
#include <math.h>

Vu_Meter::Vu_Meter()
{
}

Vu_Meter::Vu_Meter(const String &name)
    : Component(name)
{
}

void Vu_Meter::set_value(double value)
{
    if (value_ == value)
        return;
    value_ = value;
    repaint();
}

void Vu_Meter::paint(Graphics &g)
{
    Rectangle<int> bounds = getLocalBounds();
    double value = value_;

    double logvalue = 0;
    if (false)
        logvalue = value;
    else if (value > 0) {
        double db = 20 * log10(value);
        const double dbmin = -60.0;
        logvalue = (db - dbmin) / (0 - dbmin);
    }

    bounds.reduce(2, 1);
    int x = bounds.getX();
    int y = bounds.getY();
    int w = bounds.getWidth();
    int h = bounds.getHeight();

    const double hue_start = 210.0 / 360.0;
    const double hue_range = -240.0 / 360.0;

    int w2 = lround(w * logvalue);
    w2 = (w2 > w) ? w : w2;
    for (int i = 0; i < w2; ++i) {
        double r = i / (double)(w - 1);
        double hue = hue_start + r * hue_range;
        g.setColour(Colour::fromHSV(hue, 1.0, 1.0, 0xff));
        g.drawLine(x + i, y, x + i, y + h - 1);
    }
}
