//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "parameter_ex.h"
#include "JuceHeader.h"

class AudioProcessorEx : public AudioProcessor,
                         public AudioParametersEx::ValueChangedListener {
public:
    using AudioProcessor::AudioProcessor;
    virtual ~AudioProcessorEx() {}
};
