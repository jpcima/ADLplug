//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/graphics_utils.h"

namespace Graphics_Utils {

void draw_placeholder(Graphics &g, Rectangle<int> bounds)
{
    g.saveState();
    g.setColour(Colour::fromRGB(0xff, 0x00, 0x00));
    g.drawRect(bounds);
    Point<int> tl = bounds.getTopLeft();
    Point<int> tr = bounds.getTopRight();
    Point<int> bl = bounds.getBottomLeft();
    Point<int> br = bounds.getBottomRight();
    g.drawLine(tl.getX(), tl.getY(), br.getX(), br.getY());
    g.drawLine(tr.getX(), tr.getY(), bl.getX(), bl.getY());
    g.restoreState();
}

}  // namespace Graphics_Utils
