//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/components/knob_component.h"
#include <cmath>

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

void Knob::set_max_increment(float maxinc)
{
    max_increment_ = maxinc;
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

    const float value = value_;
    const float ratio = (value - min_) / (max_ - min_);
    unsigned long index = lround(ratio * (frame_count - 1));
    index = ((long)index < 0) ? 0 : (index >= frame_count) ? (frame_count - 1) : index;

    g.drawImage(frames[index], get_frame_bounds());
}

void Knob::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel)
{
    if (in_drag_ || wheel.deltaY == 0)
        return;

    Rectangle<int> frame_bounds = get_frame_bounds().toType<int>();
    if (!frame_bounds.contains(event.getPosition()))
        return;

    Component::BailOutChecker checker(this);
    listeners_.callChecked(checker, [this](Knob::Listener &l) { l.knob_drag_started(this); });
    if (checker.shouldBailOut())
        return;
    const float k = 0.5f * (max_ - min_);
    float inc = k * wheel.deltaY;

    float maxinc = max_increment_;
    if (maxinc > 0)
        inc = std::copysign(std::min(std::fabs(inc), maxinc), inc);

    set_value(value_ + inc, sendNotificationSync);
    listeners_.callChecked(checker, [this](Knob::Listener &l) { l.knob_drag_ended(this); });
}

void Knob::mouseDown(const MouseEvent &event)
{
    if (in_drag_)
        return;

    Point<int> pos = event.getPosition();
    Rectangle<int> frame_bounds = get_frame_bounds().toType<int>();
    if (!frame_bounds.contains(pos))
        return;

    in_drag_ = true;
    Component::BailOutChecker checker(this);
    listeners_.callChecked(checker, [this](Knob::Listener &l) { l.knob_drag_started(this); });

    handle_drag(event);
}

void Knob::mouseUp(const MouseEvent &event)
{
    if (!in_drag_)
        return;

    in_drag_ = false;
    Component::BailOutChecker checker(this);
    listeners_.callChecked(checker, [this](Knob::Listener &l) { l.knob_drag_ended(this); });
}

void Knob::mouseDrag(const MouseEvent &event)
{
    if (!in_drag_)
        return;

    handle_drag(event);
}

void Knob:: handle_drag(const MouseEvent &event)
{
    Km_Style style = skin_->style;
    Rectangle<float> bounds = get_frame_bounds();

    if (style == Km_Rotary) {
        float dx = event.position.x - bounds.getCentreX();
        float dy = event.position.y - bounds.getCentreY();

        if (dx * dx + dy * dy > 25.0f) {
            float angle = std::atan2(dx, -dy);
            angle = std::max(angle, min_angle_);
            angle = std::min(angle, max_angle_);
            float r = (angle - min_angle_) / (max_angle_ - min_angle_);
            float new_value = min_ + r * (max_ - min_);
            set_value(new_value, sendNotificationSync);
        }
    }
    else if (style == Km_LinearHorizontal) {
        float dx = event.position.x - bounds.getX();
        float w = bounds.getWidth();
        double r = jlimit(0.0f, 1.0f, dx / w);
        float new_value = min_ + r * (max_ - min_);
        set_value(new_value, sendNotificationSync);
    }
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
