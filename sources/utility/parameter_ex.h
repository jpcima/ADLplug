//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"

namespace AudioParametersEx {
    class ValueChangedListener {
    public:
        virtual ~ValueChangedListener() {}
        virtual void parameterValueChangedEx(int) {}
    };
};

template <class Parameter>
class AudioParameterEx : public Parameter {
public:
    using Parameter::Parameter;
    typedef AudioParametersEx::ValueChangedListener ValueChangedListener;

    void addValueChangedListenerEx(ValueChangedListener *l);
    void removeValueChangedListenerEx(ValueChangedListener *l);

    int getTagEx() const noexcept;
    void setTagEx(int tag) noexcept;

    void setAutomatable(bool automatable);
    bool isAutomatable() const override;

protected:
    void invoke_value_changed_listeners();

private:
    CriticalSection listener_lock_;
    Array<ValueChangedListener *> listeners_;
    int tag_ = 0;
    int automatable_ = -1;
};

//------------------------------------------------------------------------------
class AudioParameterExBool : public AudioParameterEx<AudioParameterBool> {
public:
    using AudioParameterEx::AudioParameterEx;
    void valueChanged(bool) override
        { invoke_value_changed_listeners(); }
};

class AudioParameterExChoice : public AudioParameterEx<AudioParameterChoice> {
public:
    using AudioParameterEx::AudioParameterEx;
    void valueChanged(int) override
        { invoke_value_changed_listeners(); }
};

class AudioParameterExFloat : public AudioParameterEx<AudioParameterFloat> {
public:
    using AudioParameterEx::AudioParameterEx;
    void valueChanged(float) override
        { invoke_value_changed_listeners(); }
};

class AudioParameterExInt : public AudioParameterEx<AudioParameterInt> {
public:
    using AudioParameterEx::AudioParameterEx;
    void valueChanged(int) override
        { invoke_value_changed_listeners(); }
};

//------------------------------------------------------------------------------
enum class AudioParameterType {
    Bool, Choice, Float, Int,
};

//------------------------------------------------------------------------------
template <AudioParameterType Ty>
struct TypedAudioParameterTraits;

template <>
struct TypedAudioParameterTraits<AudioParameterType::Bool> {
    typedef AudioParameterExBool type;
};

template <>
struct TypedAudioParameterTraits<AudioParameterType::Choice> {
    typedef AudioParameterExChoice type;
};

template <>
struct TypedAudioParameterTraits<AudioParameterType::Float> {
    typedef AudioParameterExFloat type;
};

template <>
struct TypedAudioParameterTraits<AudioParameterType::Int> {
    typedef AudioParameterExInt type;
};

template <AudioParameterType Ty>
using TypedAudioParameter = typename TypedAudioParameterTraits<Ty>::type;

#include "parameter_ex.tcc"
