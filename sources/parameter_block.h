//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
#include "utility/parameter_ex.h"
#include <memory>
#include <utility>

struct Basic_Parameter_Block {
    template <AudioParameterType Ty, class... Arg>
    TypedAudioParameter<Ty> *addAutomatableParameter(AudioProcessor &p, Arg &&... args);

    template <AudioParameterType Ty, class... Arg>
    NonAutomatableAudioParameter<Ty> *addParameter(AudioProcessor &p, Arg &&... args);

private:
    template <class T, class... Arg>
    T *doAddParameter(AudioProcessor &p, Arg &&... args);
};

template <AudioParameterType Ty, class... Arg>
inline TypedAudioParameter<Ty> *Basic_Parameter_Block::addAutomatableParameter(AudioProcessor &p, Arg &&... args)
{
    return doAddParameter<TypedAudioParameter<Ty>>(p, std::forward<Arg>(args)...);
}

template <AudioParameterType Ty, class... Arg>
inline NonAutomatableAudioParameter<Ty> *Basic_Parameter_Block::addParameter(AudioProcessor &p, Arg &&... args)
{
    return doAddParameter<NonAutomatableAudioParameter<Ty>>(p, std::forward<Arg>(args)...);
}

template <class T, class... Arg>
inline T *Basic_Parameter_Block::doAddParameter(AudioProcessor &p, Arg &&... args)
{
    std::unique_ptr<T> parameter(new T(std::forward<Arg>(args)...));
    p.addParameter(parameter.get());
    return parameter.release();
}

#if defined(ADLPLUG_OPL3)
#include "opl3/parameter_block.h"
#elif defined(ADLPLUG_OPN2)
#include "opn2/parameter_block.h"
#endif
