//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/knob_component.h"
#include "ui/image_utils.h"

Knob::Knob()
{
}

Knob::Knob(const String &name)
    : Component(name)
{
}

void Knob::load_skin(const Image &img, unsigned frame_count)
{
    std::vector<Image> &skin = skin_;
    skin.resize(frame_count);

    if (skin.empty())
        return;

    int w = img.getWidth();
    int h = img.getHeight();
    int hframe = h / frame_count;
    Rectangle<int> frame_area (0, 0, w, hframe);
    for (unsigned i = 0; i < frame_count; ++i) {
        Rectangle<int> bounds(0, i * hframe, w, hframe);
        skin[i] = img.getClippedImage(bounds);
    }

    // crop transparent bounds
    Rectangle<int> opaque_bounds = Image_Utils::get_image_solid_area(skin[0]);
    for (unsigned i = 1; i < frame_count; ++i)
        opaque_bounds = opaque_bounds.getUnion(Image_Utils::get_image_solid_area(skin[i]));
    for (unsigned i = 0; i < frame_count; ++i)
        skin[i] = skin[i].getClippedImage(opaque_bounds);
}

void Knob::load_skin_data(const char *data, unsigned size, unsigned frame_count)
{
    Image img = ImageFileFormat::loadFrom(data, size);
    load_skin(img, frame_count);
}

void Knob::load_skin_resource(const char *name, unsigned frame_count)
{
    int size;
    const char *data = BinaryData::getNamedResource(name, size);
    if (!data)
        return;
    load_skin_data(data, size, frame_count);
}

float Knob::value() const
{
    return value_;
}

void Knob::set_value(float v)
{
    v = (v < 0) ? 0 : (v > 1) ? 1 : v;
    if (v != value_) {
        value_ = v;
        listeners_.call([this](Knob::Listener &l) { l.knob_value_changed(this); });
        repaint();
    }
}

void Knob::add_listener(Listener *l)
{
    listeners_.add(l);
}

void Knob::remove_listener(Listener *l)
{
    listeners_.remove(l);
}

void Knob::paint(Graphics &g)
{
    const std::vector<Image> &skin = skin_;
    size_t frame_count = skin.size();
    if (frame_count <= 0)
        return;

    Rectangle<int> bounds = getLocalBounds();
    int x = bounds.getX();
    int y = bounds.getY();
    int w = bounds.getWidth();
    int h = bounds.getHeight();

    const float value = value_;
    unsigned long index = lround(value * (frame_count - 1));
    index = ((long)index < 0) ? 0 : (index >= frame_count) ? (frame_count - 1) : index;

    const Image &frame = skin[index];
    g.drawImage(frame, get_frame_bounds());
}

void Knob::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel)
{
    Rectangle<int> frame_bounds = get_frame_bounds().toType<int>();
    if (wheel.deltaY && frame_bounds.contains(event.getPosition())) {
        const float k = 0.5f;
        set_value(value_ + k * wheel.deltaY);
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
        const float k = 0.5f;
        Point<int> center = frame_bounds.getCentre();
        float xamt = k * (pos - center).getX() / frame_bounds.getWidth();
        float yamt = -k * (pos - center).getY() / frame_bounds.getHeight();
        float amt = (std::abs(xamt) > std::abs(yamt)) ? xamt : yamt;
        set_value(value_at_drag_start_ + amt);
        return;
    }
    Component::mouseDrag(event);
}

Rectangle<float> Knob::get_frame_bounds() const
{
    const std::vector<Image> &skin = skin_;
    if (skin.empty())
        return Rectangle<float>();

    Rectangle<int> fbounds = skin[0].getBounds();
    return getLocalBounds().toType<float>().withSizeKeepingCentre(
        fbounds.getWidth(), fbounds.getHeight());
}
