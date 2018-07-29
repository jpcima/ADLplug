//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/components/knob_component.h"

Knob::Knob()
{
}

Knob::Knob(const String &name)
    : Component(name)
{
}

Km_Skin *Knob::skin() const
{
    return skin_.get();
}

void Knob::set_skin(Km_Skin *skin)
{
    if (skin_.get() != skin) {
        skin_ = skin;
        repaint();
    }
}

void Knob::set_value(float v, NotificationType notification)
{
    v = (v < min_) ? min_ : (v > max_) ? max_ : v;
    if (v == value_)
        return;
    value_ = v;
    repaint();

    if (notification == dontSendNotification)
        return;

    if (notification == sendNotificationSync)
        handleAsyncUpdate();
    else
        triggerAsyncUpdate();
}

void Knob::set_range(float min, float max)
{
    jassert(min < max);

    min_ = min;
    max_ = max;
    set_value(value_, dontSendNotification);
}

void Knob::add_listener(Listener *l)
{
    listeners_.add(l);
}

void Knob::remove_listener(Listener *l)
{
    listeners_.remove(l);
}

void Knob::handleAsyncUpdate()
{
    cancelPendingUpdate();

    Component::BailOutChecker checker(this);
    listeners_.callChecked(checker, [this](Knob::Listener &l) { l.knob_value_changed(this); });

    if (checker.shouldBailOut())
        return;

#if 0
    if (this->on_value_change != nullptr)
        this->on_value_change();
#endif
}

void Knob::paint(Graphics &g)
{
    const Km_Skin *skin = skin_.get();
    if (!skin || !*skin)
        return;

    const std::vector<Image> &frames = skin->frames;
    size_t frame_count = frames.size();
    Rectangle<int> bounds = getLocalBounds();
    int x = bounds.getX();
    int y = bounds.getY();
    int w = bounds.getWidth();
    int h = bounds.getHeight();

    const float value = value_;
    const float ratio = (value - min_) / (max_ - min_);
    unsigned long index = lround(ratio * (frame_count - 1));
    index = ((long)index < 0) ? 0 : (index >= frame_count) ? (frame_count - 1) : index;

    g.drawImage(frames[index], get_frame_bounds());
}

void Knob::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel)
{
    Rectangle<int> frame_bounds = get_frame_bounds().toType<int>();
    if (wheel.deltaY && frame_bounds.contains(event.getPosition())) {
        const float k = 0.5f * (max_ - min_);
        set_value(value_ + k * wheel.deltaY, sendNotification);
        return;
    }
    Component::mouseWheelMove(event, wheel);
}

void Knob::mouseDown(const MouseEvent &event)
{
    Point<int> pos = event.getPosition();
    Rectangle<int> frame_bounds = get_frame_bounds().toType<int>();
    if (event.mods.isLeftButtonDown() && frame_bounds.contains(pos)) {
        in_drag_ = true;
        value_at_drag_start_ = value_;
    }
    Component::mouseDown(event);
}

void Knob::mouseUp(const MouseEvent &event)
{
    if (in_drag_ && !event.mods.isLeftButtonDown()) {
        in_drag_ = false;
    }
    Component::mouseUp(event);
}

void Knob::mouseDrag(const MouseEvent &event)
{
    Point<int> pos = event.getPosition();
    Point<int> off = event.getOffsetFromDragStart();
    Rectangle<int> frame_bounds = get_frame_bounds().toType<int>();
    if (in_drag_ && event.mods.isLeftButtonDown()) {
        const float k = 0.5f * (max_ - min_);
        Point<int> center = frame_bounds.getCentre();
        float xamt = k * (pos - center).getX() / frame_bounds.getWidth();
        float yamt = -k * (pos - center).getY() / frame_bounds.getHeight();
        float amt = (std::abs(xamt) > std::abs(yamt)) ? xamt : yamt;
        set_value(value_at_drag_start_ + amt, sendNotification);
        return;
    }
    Component::mouseDrag(event);
}

Rectangle<float> Knob::get_frame_bounds() const
{
    const Km_Skin *skin = skin_.get();
    if (!skin || !*skin)
        return Rectangle<float>();

    const std::vector<Image> &frames = skin->frames;
    Rectangle<int> fbounds = frames[0].getBounds();
    return getLocalBounds().toType<float>().withSizeKeepingCentre(
        fbounds.getWidth(), fbounds.getHeight());
}
