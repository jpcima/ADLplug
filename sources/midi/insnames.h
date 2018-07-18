//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <unordered_map>

enum class Midi_Spec {
    GM, GS, SC88, MT32, XG,
};

const char *midi_spec_name(Midi_Spec spec);

struct Midi_Program_Ex {
    Midi_Program_Ex()
        {}
    Midi_Program_Ex(Midi_Spec spec, const char *name)
        : spec(spec), name(name) {}
    Midi_Spec spec {};
    const char *name = nullptr;
};

class Midi_Db {
public:
    void init();

    const char *inst(unsigned id7)
        { return midi_inst_[id7]; }
    const Midi_Program_Ex &perc(unsigned id7)
        { return midi_perc_[id7]; }
    const Midi_Program_Ex *find_ex(
        unsigned msb, unsigned lsb, unsigned pgm);
private:
    const char *midi_inst_[128];
    Midi_Program_Ex midi_perc_[128];
    std::unordered_map<unsigned, Midi_Program_Ex> midi_ex_;
    void init_midi_inst();
    void init_midi_perc();
    void init_midi_ex();
};

extern Midi_Db midi_db;
