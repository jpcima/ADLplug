//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/utility/image.h"

namespace Image_Utils {

Rectangle<int> get_image_solid_area(const Image &img)
{
    Rectangle<int> bounds = img.getBounds();

    auto col_transparent =
        [&](int c) -> bool {
            for (int r = 0, h = img.getHeight(); r < h; ++r)
                if (img.getPixelAt(c, r).getAlpha() > 0)
                    return false;
            return true;
        };
    auto row_transparent =
        [&](int r) -> bool {
            for (int c = 0, w = img.getWidth(); c < w; ++c)
                if (img.getPixelAt(c, r).getAlpha() > 0)
                    return false;
            return true;
        };

    while (bounds.getWidth() > 0 && col_transparent(bounds.getRight()))
        bounds.removeFromRight(1);
    while (bounds.getHeight() > 0 && row_transparent(bounds.getBottom()))
        bounds.removeFromBottom(1);

    int nleft = 0;
    int ntop = 0;
    while (nleft < bounds.getWidth() && col_transparent(nleft))
        ++nleft;
    while (ntop < bounds.getHeight() && col_transparent(ntop))
        ++ntop;

    bounds.removeFromLeft(nleft);
    bounds.removeFromTop(ntop);
    return bounds;
}

}  // namespace Image_Utils
