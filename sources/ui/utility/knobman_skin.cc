//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/utility/knobman_skin.h"
#include "ui/utility/image.h"
#include "resources.h"

void Km_Skin::load(const Image &img, unsigned frame_count)
{
    std::vector<Image> &frames = this->frames;
    frames.resize(frame_count);
    if (frame_count == 0)
        return;

    int w = img.getWidth();
    int h = img.getHeight();
    int hframe = h / frame_count;
    Rectangle<int> frame_area (0, 0, w, hframe);
    for (unsigned i = 0; i < frame_count; ++i) {
        Rectangle<int> bounds(0, i * hframe, w, hframe);
        frames[i] = img.getClippedImage(bounds);
    }

    // crop transparent bounds
    Rectangle<int> opaque_bounds = Image_Utils::get_image_solid_area(frames[0]);
    for (unsigned i = 1; i < frame_count; ++i)
        opaque_bounds = opaque_bounds.getUnion(Image_Utils::get_image_solid_area(frames[i]));
    for (unsigned i = 0; i < frame_count; ++i)
        frames[i] = frames[i].getClippedImage(opaque_bounds);
}

void Km_Skin::load_data(const char *data, unsigned size, unsigned frame_count)
{
    Image img = ImageFileFormat::loadFrom(data, size);
    load(img, frame_count);
}

void Km_Skin::load_resource(const Res::Data &data, unsigned frame_count)
{
    load_data(data.data, data.size, frame_count);
}

Km_Skin_Ptr Km_Skin::scaled(double ratio) const
{
    Km_Skin_Ptr skin = new Km_Skin;
    skin->style = this->style;

    size_t frame_count = this->frames.size();
    skin->frames.resize(frame_count);
    if (frame_count == 0)
        return skin;

    int orig_w = this->frames[0].getWidth();
    int orig_h = this->frames[0].getHeight();
    int new_w = lround(orig_w * ratio);
    int new_h = lround(orig_h * ratio);
    for (size_t i = 0; i < frame_count; ++i)
        skin->frames[i] = this->frames[i].rescaled(new_w, new_h, Graphics::highResamplingQuality);

    return skin;
}
