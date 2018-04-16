//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/wave_label.h"
#include "ui/chips/opl3_waves.h"

Wave_Label::Wave_Label()
{
}

Wave_Label::Wave_Label(const String &name)
    : Component(name)
{
}

void Wave_Label::set_wave(unsigned wave)
{
    if (wave_ == wave)
        return;
    wave_ = wave;
    repaint();
}

void Wave_Label::paint(Graphics &g)
{
    Rectangle<int> bounds = getLocalBounds();

    Rectangle<int> rect = bounds.reduced(2, 2);
    int x = rect.getX();
    int y = rect.getY();
    int w = rect.getWidth();
    int h = rect.getHeight();
    unsigned wave = wave_;

    g.setColour(Colour::fromRGBA(0xa0, 0xa0, 0xa0, 0xff));
    g.drawLine(x, y + h * 0.5, x + w - 1, y + h * 0.5);

    rect.reduce(12, 0);
    x = rect.getX();
    w = rect.getWidth();

    g.setColour(Colour::fromRGBA(0xff, 0x00, 0x00, 0xff));
    Point<float> xy_last;
    for (int i = -1; i < w; ++i) {
        double phase = i * (1.0 / (w - 1));
        double value = OPL3_Waves::compute_wave(wave, phase);
        Point<float> xy_new(x + i, y + h * 0.5 * (1.0 - value));
        if (i >= 0)
            g.drawLine(Line<float>(xy_last, xy_new), 1.5);
        xy_last = xy_new;
    }
}
