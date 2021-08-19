//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/components/vu_meter.h"
#include <cmath>

static const double default_hue_start = 210.0;
static const double default_hue_range = -240.0;

Vu_Meter::Vu_Meter()
    : hue_start_(default_hue_start / 360.0)
    , hue_range_(default_hue_range / 360.0)
{
    update_gradient();
}

Vu_Meter::Vu_Meter(const String &name)
    : Component(name)
    , hue_start_(default_hue_start / 360.0)
    , hue_range_(default_hue_range / 360.0)
{
    update_gradient();
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
    if (hue_start_ == start && hue_range_ == range)
        return;
    hue_start_ = start;
    hue_range_ = range;
    update_gradient();
    repaint();
}

void Vu_Meter::set_num_stops(unsigned num_stops)
{
    jassert(num_stops >= 2);
    if (num_stops_ == num_stops)
        return;
    num_stops_ = num_stops;
    update_gradient();
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

    int w2 = (int)std::lround(w * logvalue);
    w2 = (w2 > w) ? w : w2;

    ColourGradient gradient = gradient_;
    gradient.point1.setXY(bounds.getX(), bounds.getY());
    gradient.point2.setXY(bounds.getRight(), bounds.getY());
    g.setGradientFill(gradient);
    g.fillRect(bounds.withWidth(w2));
}

void Vu_Meter::update_gradient()
{
    ColourGradient &gradient = gradient_;
    const unsigned num_stops = num_stops_;
    const double hue_start = hue_start_;
    const double hue_range = hue_range_;

    gradient.clearColours();
    for (unsigned s = 0; s < num_stops; ++s) {
        double r = s / (double)(num_stops - 1);
        double hue = hue_start + r * hue_range;
        gradient.addColour(r, Colour::fromHSV(hue, 0.75, 0.75, 0xff));
    }
}
