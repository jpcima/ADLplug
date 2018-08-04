//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include <utility>

class Functional_Timer : public Timer {
public:
    virtual ~Functional_Timer() {}
    template <class T> static Timer *create(T fn);
};

template <class T>
class Functional_Timer_T : public Functional_Timer {
public:
    explicit Functional_Timer_T(T fn) : fn_(std::move(fn)) {}
    void timerCallback() override { fn_(); }
private:
    T fn_;
};

template <class T>
Timer *Functional_Timer::create(T fn)
{
    return new Functional_Timer_T<T>(std::move(fn));
}
