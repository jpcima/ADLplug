//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/components/algorithm_component.h"
#include "algorithms.h"

void Algorithm_Component::paint(Graphics &g)
{
    const char16_t *drawing = u"";
    switch (algorithm_) {
    case 0: drawing = Algorithms::algorithm_2fm; break;
    case 1: drawing = Algorithms::algorithm_2am; break;
    case 2: drawing = Algorithms::algorithm_4fmfm; break;
    case 3: drawing = Algorithms::algorithm_4amfm; break;
    case 4: drawing = Algorithms::algorithm_4fmam; break;
    case 5: drawing = Algorithms::algorithm_4amam; break;
    }

    double scale = scale_;
    unsigned row = 0;
    unsigned col = 0;

    Colour pen_color(0xf0, 0xf8, 0xff);
    Colour fill_color(0x42, 0xa2, 0xc8);

    g.setColour(pen_color);
    g.drawRect(getLocalBounds());

    const char16_t *end = drawing + std::char_traits<char16_t>::length(drawing);
    for (const char16_t *p = drawing; p != end; ++p) {
        char16_t character = *p;

        Rectangle<float> bounds;
        bounds.setX(scale * col);
        bounds.setY(scale * row);
        bounds.setWidth(scale);
        bounds.setHeight(scale);

        switch (character) {
        case u'│':
            g.setColour(pen_color);
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 0.0f),
                                   bounds.getRelativePoint(0.5f, 1.0f)));
            break;
        case u'─':
            g.setColour(pen_color);
            g.drawLine(Line<float>(bounds.getRelativePoint(0.0f, 0.5f),
                                   bounds.getRelativePoint(1.0f, 0.5f)));
            break;
        case u'┌':
            g.setColour(pen_color);
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 1.0f),
                                   bounds.getRelativePoint(0.5f, 0.5f)));
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 0.5f),
                                   bounds.getRelativePoint(1.0f, 0.5f)));
            break;
        case u'┐':
            g.setColour(pen_color);
            g.drawLine(Line<float>(bounds.getRelativePoint(0.0f, 0.5f),
                                   bounds.getRelativePoint(0.5f, 0.5f)));
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 0.5f),
                                   bounds.getRelativePoint(0.5f, 1.0f)));
            break;
        case u'└':
            g.setColour(pen_color);
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 0.0f),
                                   bounds.getRelativePoint(0.5f, 0.5f)));
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 0.5f),
                                   bounds.getRelativePoint(1.0f, 0.5f)));
            break;
        case u'┘':
            g.setColour(pen_color);
            g.drawLine(Line<float>(bounds.getRelativePoint(0.0f, 0.5f),
                                   bounds.getRelativePoint(0.5f, 0.5f)));
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 0.5f),
                                   bounds.getRelativePoint(0.5f, 0.0f)));
            break;
        case u'┤':
            g.setColour(pen_color);
            g.drawLine(Line<float>(bounds.getRelativePoint(0.0f, 0.5f),
                                   bounds.getRelativePoint(0.5f, 0.5f)));
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 0.0f),
                                   bounds.getRelativePoint(0.5f, 1.0f)));
            break;
        case u'┴':
            g.setColour(pen_color);
            g.drawLine(Line<float>(bounds.getRelativePoint(0.0f, 0.5f),
                                   bounds.getRelativePoint(1.0f, 0.5f)));
            g.drawLine(Line<float>(bounds.getRelativePoint(0.5f, 0.5f),
                                   bounds.getRelativePoint(0.5f, 0.0f)));
            break;
        case u' ':
        case u'\n':
            break;
        default:
            g.setColour(fill_color);
            g.fillRoundedRectangle(bounds, 5.0f);
            g.setColour(Colours::black);
            g.drawText(String(CharPointer_UTF16((const CharPointer_UTF16::CharType *)&character), 1),
                       bounds, Justification::centred, false);
        }

        col = (character != '\n') ? (col + 1) : 0;
        row += character == '\n';
    }
}
