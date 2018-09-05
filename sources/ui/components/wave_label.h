//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
class Waves;

class Wave_Label : public Component,
                   public AsyncUpdater
{
public:
    explicit Wave_Label(const Waves &waves);
    explicit Wave_Label(const Waves &waves, const String &name);

    unsigned wave() const
        { return wave_; }
    void set_wave(unsigned wave, NotificationType notification);

    class Listener {
    public:
        virtual ~Listener() {}
        virtual void wave_changed(Wave_Label *k) {}
    };

    void add_listener(Listener *l);
    void remove_listener(Listener *l);

#if 0
    std::function<void()> on_wave_change;
#endif

protected:
    void handleAsyncUpdate() override;
    void paint(Graphics &g) override;

private:
    const Waves &waves_;
    unsigned wave_ = 0;
    ListenerList<Listener> listeners_;
};
