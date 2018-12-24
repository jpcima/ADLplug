//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "parameter_block.h"
#include <utility>

template <AudioParameterType Ty, class... Arg>
inline TypedAudioParameter<Ty> *Basic_Parameter_Block::add_automatable_parameter(AudioProcessorEx &p, int tag, Arg &&... args)
{
    TypedAudioParameter<Ty> *par = do_add_parameter<TypedAudioParameter<Ty>>(p, tag, std::forward<Arg>(args)...);
    par->setAutomatable(true);
    return par;
}

template <AudioParameterType Ty, class... Arg>
inline TypedAudioParameter<Ty> *Basic_Parameter_Block::add_parameter(AudioProcessorEx &p, int tag, Arg &&... args)
{
    TypedAudioParameter<Ty> *par = do_add_parameter<TypedAudioParameter<Ty>>(p, tag, std::forward<Arg>(args)...);
    par->setAutomatable(false);
    return par;
}

template <AudioParameterType Ty, class... Arg>
inline TypedAudioParameter<Ty> *Basic_Parameter_Block::add_internal_parameter(AudioProcessorEx &p, int tag, Arg &&... args)
{
    TypedAudioParameter<Ty> *par = do_add_internal_parameter<TypedAudioParameter<Ty>>(p, tag, std::forward<Arg>(args)...);
    par->setAutomatable(false);
    return par;
}

template <class T, class... Arg>
inline T *Basic_Parameter_Block::do_add_parameter(AudioProcessorEx &p, int tag, Arg &&... args)
{
    std::unique_ptr<T> parameter(new T(std::forward<Arg>(args)...));
    tag_of_external_parameter_.push_back(tag);
    p.addParameter(parameter.get());
    parameter->setTagEx(tag);
    parameter->addValueChangedListenerEx(&p);
    return parameter.release();
}

template <class T, class... Arg>
inline T *Basic_Parameter_Block::do_add_internal_parameter(AudioProcessorEx &p, int tag, Arg &&... args)
{
    std::unique_ptr<T> parameter(new T(std::forward<Arg>(args)...));
    internal_parameters_.emplace_back(parameter.get());
    parameter->setTagEx(tag);
    parameter->addValueChangedListenerEx(&p);
    return parameter.release();
}
