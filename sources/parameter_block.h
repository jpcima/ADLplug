//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

struct Parameter_Block {
    AudioParameterBool *p_4op = nullptr;
    AudioParameterBool *p_ps4op = nullptr;
    AudioParameterBool *p_blank = nullptr;
    AudioParameterChoice *p_con12 = nullptr;
    AudioParameterChoice *p_con34 = nullptr;
    AudioParameterInt *p_tune12 = nullptr;
    AudioParameterInt *p_tune34 = nullptr;
    AudioParameterInt *p_fb12 = nullptr;
    AudioParameterInt *p_fb34 = nullptr;
    AudioParameterInt *p_veloffset = nullptr;
    AudioParameterInt *p_voice2ft = nullptr;
    AudioParameterInt *p_drumnote = nullptr;

    struct Operator {
        AudioParameterInt *p_attack = nullptr;
        AudioParameterInt *p_decay = nullptr;
        AudioParameterInt *p_sustain = nullptr;
        AudioParameterInt *p_release = nullptr;
        AudioParameterInt *p_level = nullptr;
        AudioParameterInt *p_ksl = nullptr;
        AudioParameterInt *p_fmul = nullptr;
        AudioParameterBool *p_trem = nullptr;
        AudioParameterBool *p_vib = nullptr;
        AudioParameterBool *p_sus = nullptr;
        AudioParameterBool *p_env = nullptr;
        AudioParameterInt *p_wave = nullptr;
    };

    Operator c1, m1, c2, m2;

    Operator &nth_operator(unsigned i)
        { Operator *ops[] = {&c1, &m1, &c2, &m2}; return *ops[i]; }
    const Operator &nth_operator(unsigned i) const
        { return const_cast<Parameter_Block *>(this)->nth_operator(i); }

    void setup_parameters(AudioProcessor &p);
};
