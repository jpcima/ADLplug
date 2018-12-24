//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "parameter_ex.h"

template <class Parameter>
void AudioParameterEx<Parameter>::addValueChangedListenerEx(ValueChangedListener *l)
{
    const ScopedLock sl(listener_lock_);
    listeners_.addIfNotAlreadyThere(l);
}

template <class Parameter>
void AudioParameterEx<Parameter>::removeValueChangedListenerEx(ValueChangedListener *l)
{
    const ScopedLock sl(listener_lock_);
    listeners_.removeFirstMatchingValue(l);
}

template <class Parameter>
void AudioParameterEx<Parameter>::invoke_value_changed_listeners()
{
    const ScopedLock sl(listener_lock_);
    for (unsigned i = listeners_.size(); i-- > 0;)
        listeners_[i]->parameterValueChangedEx(tag_);
}

template <class Parameter>
int AudioParameterEx<Parameter>::getTagEx() const noexcept
{
    return tag_;
}

template <class Parameter>
void AudioParameterEx<Parameter>::setTagEx(int tag) noexcept
{
    tag_ = tag;
}

template <class Parameter>
void AudioParameterEx<Parameter>:: setAutomatable(bool automatable)
{
    automatable_ = automatable;
}

template <class Parameter>
bool AudioParameterEx<Parameter>::isAutomatable() const
{
    switch (automatable_) {
    default:
        return Parameter::isAutomatable();
    case 0:
        return false;
    case 1:
        return true;
    }
}
