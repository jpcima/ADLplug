//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
#include "utility/processor_ex.h"
#include "utility/parameter_ex.h"
#include <vector>
#include <memory>

struct Basic_Parameter_Block {
    template <AudioParameterType Ty, class... Arg>
    TypedAudioParameter<Ty> *add_automatable_parameter(AudioProcessorEx &p, int tag, Arg &&... args);

    template <AudioParameterType Ty, class... Arg>
    TypedAudioParameter<Ty> *add_parameter(AudioProcessorEx &p, int tag, Arg &&... args);

    template <AudioParameterType Ty, class... Arg>
    TypedAudioParameter<Ty> *add_internal_parameter(AudioProcessorEx &p, int tag, Arg &&... args);

    int external_parameter_tag(unsigned parameter_index) const
        { return tag_of_external_parameter_[parameter_index]; }

private:
    template <class T, class... Arg>
    T *do_add_parameter(AudioProcessorEx &p, int tag, Arg &&... args);

    template <class T, class... Arg>
    T *do_add_internal_parameter(AudioProcessorEx &p, int tag, Arg &&... args);

    std::vector<std::unique_ptr<AudioProcessorParameter>> internal_parameters_;
    std::vector<int> tag_of_external_parameter_;
};

#if defined(ADLPLUG_OPL3)
#include "opl3/parameter_block.h"
#elif defined(ADLPLUG_OPN2)
#include "opn2/parameter_block.h"
#endif

#include "parameter_block.tcc"
