//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/components/indicator_NxM.h"

Indicator_NxM::Indicator_NxM(unsigned rows, unsigned cols)
    : rows_(rows), bits_(rows * cols)
{
}

bool Indicator_NxM::value(unsigned row, unsigned col)
{
    unsigned index = index_from(row, col);
    if (index >= bits_.size())
        return false;
    return bits_[index];
}

void Indicator_NxM::set_value(unsigned row, unsigned col, bool value)
{
    unsigned index = index_from(row, col);
    if (index >= bits_.size())
        return;
    if (bits_[index] != value) {
        bits_[index] = value;
        repaint();
    }
}

void Indicator_NxM::paint(Graphics &g)
{
    Rectangle<float> bounds = getLocalBounds().toType<float>();
    LookAndFeel &lnf = getLookAndFeel();

    Colour colour_on = Colour::fromRGBA(0xdf, 0xf0, 0xff, 0xff);
    Colour colour_off = lnf.findColour(Label::backgroundColourId);
    Colour colour_outline = Colour::fromRGBA(0x8e, 0x98, 0x9b, 0xff);

    unsigned rows = this->rows();
    unsigned columns = this->columns();
    if (rows == 0 || columns == 0)
        return;

    float w1 = bounds.getWidth() / columns;
    float h1 = bounds.getHeight() / rows;
    for (unsigned r = 0; r < rows; ++r) {
        for (unsigned c = 0; c < columns; ++c) {
            Point<float> origin =
                bounds.getTopLeft() + Point<float>(c * w1, r * h1);
            Rectangle<float> rect(origin.getX(), origin.getY(), w1, h1);
            rect.reduce(1, 1);
            bool value = this->value(r, c);
            Colour colour = value ? colour_on : colour_off;
            g.setColour(colour);
            g.fillRect(rect);
            g.setColour(colour_outline);
            g.drawRect(rect);
        }
    }
}

unsigned Indicator_NxM::index_from(unsigned row, unsigned column)
{
    return rows_ * column + row;
}
