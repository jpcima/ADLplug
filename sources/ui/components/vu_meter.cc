//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/components/vu_meter.h"
#include <cmath>

static const double default_hue_start = 210.0;
static const double default_hue_range = -240.0;

Vu_Meter::Vu_Meter()
{
    set_hue(default_hue_start, default_hue_range);
}

Vu_Meter::Vu_Meter(const String &name)
    : Component(name)
{
    set_hue(default_hue_start, default_hue_range);
}

void Vu_Meter::set_value(double value)
{
    if (value_ == value)
        return;
    value_ = value;
    repaint();
}

void Vu_Meter::set_logarithmic(bool logarithmic)
{
    if (logarithmic_ == logarithmic)
        return;
    logarithmic_ = logarithmic;
    repaint();
}

void Vu_Meter::set_hue(double start, double range)
{
    start *= 1.0 / 360.0;
    range *= 1.0 / 360.0;
    hue_start_ = start;
    hue_range_ = range;
    repaint();
}

void Vu_Meter::paint(Graphics &g)
{
    Rectangle<int> bounds = getLocalBounds();
    double value = value_;

    double logvalue = 0;
    if (!logarithmic_)
        logvalue = value;
    else if (value > 0) {
        double db = 20 * std::log10(value);
        const double dbmin = -60.0;
        logvalue = (db - dbmin) / (0 - dbmin);
    }

    bounds.reduce(1, 1);
    int x = bounds.getX();
    int y = bounds.getY();
    int w = bounds.getWidth();
    int h = bounds.getHeight();

    if (w <= 0)
        return;

    const double hue_start = hue_start_;
    const double hue_range = hue_range_;
    std::vector<Colour> &colormap = colormap_;

    if (colormap.size() != (unsigned)w) {
        colormap.resize((unsigned)w);
        for (int i = 0; i < w; ++i) {
            double r = i / (double)w;
            double hue = hue_start + r * hue_range;
            colormap[i] = Colour::fromHSV(hue, 0.75, 0.75, 0xff);
        }
    }

    int w2 = (int)std::lround(w * logvalue);
    w2 = (w2 > w) ? w : w2;
    for (int i = 0; i < w2; ++i) {
        g.setColour(colormap[i]);
        g.drawVerticalLine(x + i, y, y + h);
    }
}
