//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "ui/knobman_skin.h"
#include "../../JuceLibraryCode/JuceHeader.h"

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

    class Listener {
    public:
        virtual ~Listener() {}
        virtual void knob_value_changed(Knob *k) {}
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
    Rectangle<float> get_frame_bounds() const;
    Km_Skin_Ptr skin_;
    float value_ = 0;
    float min_ = 0;
    float max_ = 1;
    ListenerList<Listener> listeners_;
    bool in_drag_ = false;
    float value_at_drag_start_ = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob);
};
