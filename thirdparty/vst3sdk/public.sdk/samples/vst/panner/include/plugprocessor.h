//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : plugprocessor.h
// Created by  : Steinberg, 02/2020
// Description : Panner Example for VST 3
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2020, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
namespace Panner {

//-----------------------------------------------------------------------------
class PlugProcessor : public Vst::AudioEffect
{
public:
	PlugProcessor ();

	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API setBusArrangements (Vst::SpeakerArrangement* inputs, int32 numIns,
	                                       Vst::SpeakerArrangement* outputs,
	                                       int32 numOuts) SMTG_OVERRIDE;

	tresult PLUGIN_API canProcessSampleSize (int32 symbolicSampleSize) SMTG_OVERRIDE;
	tresult PLUGIN_API setupProcessing (Vst::ProcessSetup& setup) SMTG_OVERRIDE;
	tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;
	tresult PLUGIN_API process (Vst::ProcessData& data) SMTG_OVERRIDE;

//------------------------------------------------------------------------
	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;

	static FUnknown* createInstance (void*) { return (Vst::IAudioProcessor*)new PlugProcessor (); }

	enum
	{
		kPanLawEqualPower = 0,
	};

protected:
	void getStereoPanCoef (int32 panType, float pan, float& left, float& right) const;
	
	template <typename SampleType>
	tresult processAudio (Vst::ProcessData& data);
	tresult (PlugProcessor::*processAudioPtr) (Vst::ProcessData& data);
		
	Vst::ParamValue mPanValue = 0;

	bool mBypass = false;
};

//------------------------------------------------------------------------
} // namespace Panner
} // namespace Steinberg
