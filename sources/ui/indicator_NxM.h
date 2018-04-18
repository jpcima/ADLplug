//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include <vector>

class Indicator_NxM : public Component
{
public:
    Indicator_NxM(unsigned rows, unsigned cols);

    bool value(unsigned row, unsigned column);
    void set_value(unsigned row, unsigned column, bool value);

    unsigned rows() const
        { return rows_; }
    unsigned columns() const
        { return bits_.size() / rows_; }

protected:
    void paint(Graphics &g) override;

private:
    unsigned rows_;
    std::vector<bool> bits_;
    unsigned index_from(unsigned row, unsigned column);
};
