//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "ui/utility/knobman_skin.h"
#include "JuceHeader.h"

class Knob : public Component,
             public AsyncUpdater,
             public SettableTooltipClient
{
public:
    Knob();
    explicit Knob(const String &name);
    virtual ~Knob() {}

    Km_Skin *skin() const;
    void set_skin(Km_Skin *skin);

    float value() const
        { return value_; }
    void set_value(float v, NotificationType notification);

    float min() const
        { return min_; }
    float max() const
        { return max_; }
    void set_range(float min, float max);

    void set_max_increment(float maxinc);

    bool is_dragging() const
        { return in_drag_; }

    class Listener {
    public:
        virtual ~Listener() {}
        virtual void knob_value_changed(Knob *k) {}
        virtual void knob_drag_started(Knob *k) {}
        virtual void knob_drag_ended(Knob *k) {}
    };

    void add_listener(Listener *l);
    void remove_listener(Listener *l);

#if 0
    std::function<void()> on_value_change;
#endif

protected:
    void handleAsyncUpdate() override;
    void paint(Graphics &g) override;
    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    void mouseDown(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;

private:
    void handle_drag(const MouseEvent &event);
    Rectangle<float> get_frame_bounds() const;
    Km_Skin_Ptr skin_;
    float value_ = 0;
    float min_ = 0;
    float max_ = 1;
    float max_increment_ = 0;
    ListenerList<Listener> listeners_;
    bool in_drag_ = false;
    float min_angle_ = M_PI * -3.0 / 4.0;
    float max_angle_ = M_PI * +3.0 / 4.0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob);
};
