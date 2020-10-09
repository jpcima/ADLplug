//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/syncdelay/source/syncdelayprocessor.h
// Created by  : Steinberg, 01/2020
// Description :
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
#include "public.sdk/source/vst/vstbypassprocessor.h"

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
class SyncDelayProcessor : public AudioEffect
{
public:
	SyncDelayProcessor ();

	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API setBusArrangements (SpeakerArrangement* inputs, int32 numIns,
	                                       SpeakerArrangement* outputs,
	                                       int32 numOuts) SMTG_OVERRIDE;

	tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;
	tresult PLUGIN_API process (ProcessData& data) SMTG_OVERRIDE;

//------------------------------------------------------------------------
	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;

	static FUnknown* createInstance (void*)
	{
		return static_cast<IAudioProcessor*> (new SyncDelayProcessor ());
	}

protected:
	void doParameterChanges (IParameterChanges& changes);
	void calculateDelay ();

	BypassProcessor<Vst::Sample32> mBypassProcessor;

	uint32 mDelayIndex {0};
	uint32 mDelayInSamples {1};
	uint32 mBufferSizeInSamples {0};
	double mTempo {120};
	float** mBuffer {nullptr};
	uint32 mBufferPos {0};
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
