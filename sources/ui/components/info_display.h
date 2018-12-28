//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"

class Info_Display : protected Timer {
public:
    virtual ~Info_Display() {}

    void set_default_info(const String &text);
    void display_info(const String &text);
    void expire_info_in(unsigned timeout = 3000);

protected:
    virtual void display_info_now(const String &text) = 0;

private:
    void timerCallback() override;
    String default_text_;
};

//==============================================================================
inline void Info_Display::set_default_info(const String &text)
{
    default_text_ = text;
}

inline void Info_Display::display_info(const String &text)
{
    stopTimer();
    display_info_now(text);
}

inline void Info_Display::expire_info_in(unsigned timeout)
{
    startTimer(timeout);
}

inline void Info_Display::timerCallback()
{
    stopTimer();
    display_info_now(default_text_);
}
