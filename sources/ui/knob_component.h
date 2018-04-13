//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include <vector>

class Knob : public Component
{
public:
    Knob();
    explicit Knob(const String &name);
    void load_skin(const Image &img, unsigned frame_count);
    void load_skin_data(const char *data, unsigned size, unsigned frame_count);
    void load_skin_resource(const char *name, unsigned frame_count);

    float value() const;
    void set_value(float v);

    class Listener {
    public:
        virtual ~Listener() {}
        virtual void knob_value_changed(Knob *k) {}
    };

    void add_listener(Listener *l);
    void remove_listener(Listener *l);

protected:
    void paint(Graphics &g) override;
    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    void mouseDown(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;

private:
    Rectangle<float> get_frame_bounds() const;
    std::vector<Image> skin_;
    float value_ = 0;
    ListenerList<Listener> listeners_;
    bool in_drag_ = false;
    float value_at_drag_start_ = 0;
};
