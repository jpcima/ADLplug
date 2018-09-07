//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"

class Algorithm_Component : public Component {
public:
    explicit Algorithm_Component(const char16_t *algorithm)
        : algorithm_(algorithm) {}

    void scale(double s);
    void paint(Graphics &g) override;

private:
    const char16_t *algorithm_ = 0;
    double scale_ = 24.0;
};
