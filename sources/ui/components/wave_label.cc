//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "wave_label.h"
#include "waves.h"

Wave_Label::Wave_Label(const Waves &waves)
    : waves_(waves)
{
}

Wave_Label::Wave_Label(const Waves &waves, const String &name)
    : Component(name), waves_(waves)
{
}

void Wave_Label::set_wave(unsigned wave, NotificationType notification)
{
    if (wave_ == wave)
        return;
    wave_ = wave;
    repaint();

    if (notification == dontSendNotification)
        return;

    if (notification == sendNotificationSync)
        handleAsyncUpdate();
    else
        triggerAsyncUpdate();
}

void Wave_Label::handleAsyncUpdate()
{
    cancelPendingUpdate();

    Component::BailOutChecker checker(this);
    listeners_.callChecked(checker, [this](Wave_Label::Listener &l) { l.wave_changed(this); });

    if (checker.shouldBailOut())
        return;

#if 0
    if (this->on_wave_change != nullptr)
        this->on_wave_change();
#endif
}

void Wave_Label::paint(Graphics &g)
{
    Rectangle<int> bounds = getLocalBounds();

    Rectangle<int> rect = bounds.reduced(2, 2);
    int x = rect.getX();
    int y = rect.getY();
    int w = rect.getWidth();
    int h = rect.getHeight();
    const Waves &waves = waves_;
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
        double value = waves.compute_wave(wave, phase);
        Point<float> xy_new(x + i, y + h * 0.5 * (1.0 - value));
        if (i >= 0)
            g.drawLine(Line<float>(xy_last, xy_new), 1.5);
        xy_last = xy_new;
    }
}
