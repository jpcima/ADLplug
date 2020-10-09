//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/syncdelay/source/syncdelayprocessor.cpp
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

#include "syncdelayprocessor.h"
#include "sync.h"
#include "syncdelayids.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace Steinberg {
namespace Vst {

static constexpr auto maxDelaySeconds = 5;

//-----------------------------------------------------------------------------
SyncDelayProcessor::SyncDelayProcessor ()
{
	setControllerClass (SyncDelayControllerUID);
	processContextRequirements.needTempo ();
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SyncDelayProcessor::initialize (FUnknown* context)
{
	auto result = AudioEffect::initialize (context);
	if (result == kResultTrue)
	{
		addAudioInput (STR16 ("AudioInput"), SpeakerArr::kStereo);
		addAudioOutput (STR16 ("AudioOutput"), SpeakerArr::kStereo);
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SyncDelayProcessor::setBusArrangements (SpeakerArrangement* inputs, int32 numIns,
                                                           SpeakerArrangement* outputs,
                                                           int32 numOuts)
{
	// we only support one in and output bus and these busses must have the same number of channels
	if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0])
		return AudioEffect::setBusArrangements (inputs, numIns, outputs, numOuts);
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SyncDelayProcessor::setActive (TBool state)
{
	SpeakerArrangement arr;
	if (getBusArrangement (kOutput, 0, arr) != kResultTrue)
		return kResultFalse;
	auto numChannels = SpeakerArr::getChannelCount (arr);
	if (numChannels == 0)
		return kResultFalse;

	if (state)
	{
		mBuffer = static_cast<float**> (std::malloc (numChannels * sizeof (float*)));
		// use a maximum of 5 seconds delay time
		mBufferSizeInSamples =
		    static_cast<uint32> (std::ceil (processSetup.sampleRate) * maxDelaySeconds);
		auto size = static_cast<size_t> (mBufferSizeInSamples * sizeof (float));
		for (int32 channel = 0; channel < numChannels; channel++)
		{
			mBuffer[channel] = static_cast<float*> (std::malloc (size));
			memset (mBuffer[channel], 0, size);
		}
		mBufferPos = 0;
		mDelayIndex = FromNormalized<ParamValue> (0, static_cast<int32>(Synced.size () - 1));
		mBypassProcessor.setup (*this, processSetup, 0);
	}
	else
	{
		if (mBuffer)
		{
			for (int32 channel = 0; channel < numChannels; channel++)
			{
				std::free (mBuffer[channel]);
			}
			std::free (mBuffer);
			mBuffer = nullptr;
		}
		mBypassProcessor.reset ();
	}
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
void SyncDelayProcessor::calculateDelay ()
{
	mDelayInSamples = (60. / mTempo) * Synced[mDelayIndex].value * processSetup.sampleRate;
	mDelayInSamples = Bound (uint32(1), mBufferSizeInSamples, mDelayInSamples);
}

//------------------------------------------------------------------------
void SyncDelayProcessor::doParameterChanges (IParameterChanges& changes)
{
	auto numParamsChanged = changes.getParameterCount ();
	for (int32 index = 0; index < numParamsChanged; index++)
	{
		if (auto queue = changes.getParameterData (index))
		{
			ParamValue value;
			int32 sampleOffset;
			auto numPoints = queue->getPointCount ();
			switch (queue->getParameterId ())
			{
				case kDelayId:
				{
					if (queue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
					{
						mDelayIndex = FromNormalized<ParamValue> (value, static_cast<int32>(Synced.size () - 1));
					}
					break;
				}
				case kBypassId:
				{
					if (queue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
					{
						mBypassProcessor.setActive (value > 0.5);
					}
					break;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API SyncDelayProcessor::process (ProcessData& data)
{
	if (data.processContext && data.processContext->state & ProcessContext::kTempoValid)
	{
		if (data.processContext->tempo != mTempo)
		{
			mTempo = data.processContext->tempo;
		}
	}
	if (data.inputParameterChanges)
	{
		doParameterChanges (*data.inputParameterChanges);
	}

	if (mBypassProcessor.isActive ())
	{
		mBypassProcessor.process (data);
	}
	else if (data.numSamples > 0)
	{
		calculateDelay ();

		SpeakerArrangement arr;
		getBusArrangement (kOutput, 0, arr);
		auto numChannels = SpeakerArr::getChannelCount (arr);

		for (int32 channel = 0; channel < numChannels; channel++)
		{
			auto inputChannel = data.inputs[0].channelBuffers32[channel];
			auto outputChannel = data.outputs[0].channelBuffers32[channel];

			auto tempBufferPos = mBufferPos;
			for (int32 sampleIndex = 0; sampleIndex < data.numSamples; sampleIndex++)
			{
				auto sampleValue = inputChannel[sampleIndex];
				outputChannel[sampleIndex] = mBuffer[channel][tempBufferPos];
				mBuffer[channel][tempBufferPos] = sampleValue;
				tempBufferPos++;
				if (tempBufferPos >= mDelayInSamples)
					tempBufferPos = 0;
			}
		}
		mBufferPos += data.numSamples;
		while (mDelayInSamples && mBufferPos >= mDelayInSamples)
			mBufferPos -= mDelayInSamples;
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SyncDelayProcessor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);
	uint32 savedDelayIndex = 0;
	if (streamer.readInt32u (savedDelayIndex) == false)
		return kResultFalse;
	int32 savedBypassState = 0;
	if (streamer.readInt32 (savedBypassState) == false)
		return kResultFalse;

	mDelayIndex = savedDelayIndex;
	mBypassProcessor.setActive (savedBypassState > 0);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SyncDelayProcessor::getState (IBStream* state)
{
	IBStreamer streamer (state, kLittleEndian);

	streamer.writeInt32u (mDelayIndex);
	streamer.writeInt32 (mBypassProcessor.isActive () ? 1 : 0);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
