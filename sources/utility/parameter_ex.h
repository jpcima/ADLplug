//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"

enum class AudioParameterType {
    Bool, Choice, Float, Int,
};

//------------------------------------------------------------------------------
template <AudioParameterType Ty>
struct TypedAudioParameterTraits;

template <>
struct TypedAudioParameterTraits<AudioParameterType::Bool> {
    typedef AudioParameterBool type;
};

template <>
struct TypedAudioParameterTraits<AudioParameterType::Choice> {
    typedef AudioParameterChoice type;
};

template <>
struct TypedAudioParameterTraits<AudioParameterType::Float> {
    typedef AudioParameterFloat type;
};

template <>
struct TypedAudioParameterTraits<AudioParameterType::Int> {
    typedef AudioParameterInt type;
};

template <AudioParameterType Ty>
using TypedAudioParameter = typename TypedAudioParameterTraits<Ty>::type;

//------------------------------------------------------------------------------
template <AudioParameterType Ty>
class NonAutomatableAudioParameter : public TypedAudioParameterTraits<Ty>::type {
public:
    using TypedAudioParameterTraits<Ty>::type::type;
    virtual bool isAutomatable() const override
        { return false; }
};

typedef NonAutomatableAudioParameter<AudioParameterType::Bool> NonAutomatableAudioParameterBool;
typedef NonAutomatableAudioParameter<AudioParameterType::Choice> NonAutomatableAudioParameterChoice;
typedef NonAutomatableAudioParameter<AudioParameterType::Float> NonAutomatableAudioParameterFloat;
typedef NonAutomatableAudioParameter<AudioParameterType::Int> NonAutomatableAudioParameterInt;
