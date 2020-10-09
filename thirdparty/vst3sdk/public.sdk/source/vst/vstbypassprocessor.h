//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstbypassprocessor.h
// Created by  : Steinberg, 04/2015
// Description : Example of bypass support Implementation
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
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "vstspeakerarray.h"

namespace Steinberg {
namespace Vst {

#define kMaxChannelsSupported 64

//------------------------------------------------------------------------
// AudioBuffer
//------------------------------------------------------------------------
template <typename T>
class AudioBuffer
{
public:
//------------------------------------------------------------------------
	AudioBuffer () : mBuffer (nullptr), mMaxSamples (0) {}

	~AudioBuffer () { resize (0); }

	//--- ---------------------------------------------------------------------
	void resize (int32 _maxSamples)
	{
		if (mMaxSamples != _maxSamples)
		{
			mMaxSamples = _maxSamples;
			if (mMaxSamples <= 0)
			{
				if (mBuffer)
				{
					free (mBuffer);
					mBuffer = nullptr;
				}
			}
			else
			{
				if (mBuffer)
					mBuffer = (T*)realloc (mBuffer, mMaxSamples * sizeof (T));
				else
					mBuffer = (T*)malloc (mMaxSamples * sizeof (T));
			}
		}
	}

	//--- ---------------------------------------------------------------------
	void clear (int32 numSamples)
	{
		if (mBuffer)
		{
			int32 count = numSamples < mMaxSamples ? numSamples : mMaxSamples;
			memset (mBuffer, 0, count * sizeof (T));
		}
	}

	int32 getMaxSamples () const { return mMaxSamples; }

	void release () { resize (0); }
	void clearAll ()
	{
		if (mMaxSamples > 0)
			clear (mMaxSamples);
	}

	operator T* () { return mBuffer; }
//------------------------------------------------------------------------
protected:
	T* mBuffer;
	int32 mMaxSamples;
};

//------------------------------------------------------------------------
template <typename T>
static bool delay (int32 sampleFrames, T* inStream, T* outStream, T* delayBuffer, int32 bufferSize,
                   int32 bufferInPos, int32 bufferOutPos)
{
	// delay inStream
	int32 remain, inFrames, outFrames;
	T* bufIn;
	T* bufOut;

	remain = sampleFrames;
	while (remain > 0)
	{
		bufIn = delayBuffer + bufferInPos;
		bufOut = delayBuffer + bufferOutPos;

		if (bufferInPos > bufferOutPos)
			inFrames = bufferSize - bufferInPos;
		else
			inFrames = bufferOutPos - bufferInPos;

		outFrames = bufferSize - bufferOutPos;

		if (inFrames > remain)
			inFrames = remain;

		if (outFrames > inFrames)
			outFrames = inFrames;

		// order important for in-place processing!
		memcpy (bufIn, inStream, inFrames * sizeof (T)); // copy to buffer
		memcpy (outStream, bufOut, outFrames * sizeof (T)); // copy from buffer

		inStream += inFrames;
		outStream += outFrames;

		bufferInPos += inFrames;
		if (bufferInPos >= bufferSize)
			bufferInPos -= bufferSize;
		bufferOutPos += outFrames;
		if (bufferOutPos >= bufferSize)
			bufferOutPos -= bufferSize;

		if (inFrames > outFrames)
		{
			// still some output to copy
			bufOut = delayBuffer + bufferOutPos;
			outFrames = inFrames - outFrames;

			memcpy (outStream, bufOut, outFrames * sizeof (T)); // copy from buffer

			outStream += outFrames;

			bufferOutPos += outFrames;
			if (bufferOutPos >= bufferSize)
				bufferOutPos -= bufferSize;
		}

		remain -= inFrames;
	}

	return true;
}

//------------------------------------------------------------------------
// BypassProcessor
//------------------------------------------------------------------------
template <typename T>
class BypassProcessor
{
public:
//------------------------------------------------------------------------
	BypassProcessor () : mActive (false), mMainIOBypass (false)
	{
		for (int32 i = 0; i < kMaxChannelsSupported; i++)
		{
			mInputPinLookup[i] = -1;
			mDelays[i] = nullptr;
		}
	}
	~BypassProcessor () { reset (); }

	void setup (IAudioProcessor& audioProcessor, ProcessSetup& processSetup, int32 delaySamples)
	{
		reset ();

		SpeakerArrangement inputArr = 0;
		bool hasInput = audioProcessor.getBusArrangement (kInput, 0, inputArr) == kResultOk;

		SpeakerArrangement outputArr = 0;
		bool hasOutput = audioProcessor.getBusArrangement (kOutput, 0, outputArr) == kResultOk;

		mMainIOBypass = hasInput && hasOutput;
		if (!mMainIOBypass)
			return;

		// create lookup table (in <- out) and delays...
		SpeakerArray inArray (inputArr);
		SpeakerArray outArray (outputArr);

		// security check (todo)
		if (outArray.total () >= kMaxChannelsSupported)
			return;

		for (int32 i = 0; i < outArray.total (); i++)
		{
			if (outArray.at (i) == Vst::kSpeakerL)
			{
				if (inArray.total () == 1 && inArray.at (0) == Vst::kSpeakerM)
				{
					mInputPinLookup[i] = 0;
				}
				else
					mInputPinLookup[i] = inArray.getSpeakerIndex (outArray.at (i));
			}
			else
				mInputPinLookup[i] = inArray.getSpeakerIndex (outArray.at (i));

			mDelays[i] = new Delay (processSetup.maxSamplesPerBlock, delaySamples);
			mDelays[i]->flush ();
		}
	}
	void reset ()
	{
		mMainIOBypass = false;

		for (int32 i = 0; i < kMaxChannelsSupported; i++)
		{
			mInputPinLookup[i] = -1;
			if (mDelays[i])
			{
				delete mDelays[i];
				mDelays[i] = nullptr;
			}
		}
	}

	bool isActive () const { return mActive; }
	void setActive (bool state)
	{
		if (mActive == state)
			return;

		mActive = state;

		// flush delays when turning on
		if (state && mMainIOBypass)
			for (int32 i = 0; i < kMaxChannelsSupported; i++)
			{
				if (!mDelays[i])
					break;
				mDelays[i]->flush ();
			}
	}

	void process (ProcessData& data)
	{
		// flush
		if (data.numInputs == 0 || data.numOutputs == 0)
			return;

		AudioBusBuffers& inBus = data.inputs[0];
		AudioBusBuffers& outBus = data.outputs[0];

		if (data.symbolicSampleSize == kSample32)
		{
			if (!outBus.channelBuffers32)
				return;
		}
		else if (!outBus.channelBuffers64)
			return;

		if (mMainIOBypass)
		{
			for (int32 channel = 0; channel < outBus.numChannels; channel++)
			{
				T* src = nullptr;
				bool silent = true;
				T* dst;
				if (data.symbolicSampleSize == kSample32)
					dst = (T*)outBus.channelBuffers32[channel];
				else
					dst = (T*)outBus.channelBuffers64[channel];
				if (!dst)
					continue;

				int inputChannel = mInputPinLookup[channel];
				if (inputChannel != -1)
				{
					silent = (inBus.silenceFlags & (1ll << inputChannel)) != 0;
					if (data.symbolicSampleSize == kSample32)
						src = (T*)inBus.channelBuffers32[inputChannel];
					else
						src = (T*)inBus.channelBuffers64[inputChannel];
				}

				if (mDelays[channel]->process (src, dst, data.numSamples, silent))
				{
					outBus.silenceFlags |= (1ll << channel);
				}
				else
				{
					outBus.silenceFlags = 0;
				}
			}
		}

		// clear all other outputs
		for (int32 outBusIndex = mMainIOBypass ? 1 : 0; outBusIndex < data.numOutputs;
		     outBusIndex++)
		{
			outBus = data.outputs[outBusIndex];

			for (int32 channel = 0; channel < outBus.numChannels; channel++)
			{
				T* dst;
				if (data.symbolicSampleSize == kSample32)
					dst = (T*)outBus.channelBuffers32[channel];
				else
					dst = (T*)outBus.channelBuffers64[channel];
				if (dst)
				{
					memset (dst, 0, data.numSamples * sizeof (T));
					outBus.silenceFlags |= 1ll << channel;
				}
			}
		}
	}
//------------------------------------------------------------------------
protected:
	int32 mInputPinLookup[kMaxChannelsSupported];

	struct Delay
	{
		AudioBuffer<T> mDelayBuffer;
		int32 mDelaySamples;
		int32 mInPos;
		int32 mOutPos;

		Delay (int32 maxSamplesPerBlock, int32 delaySamples)
		: mDelaySamples (delaySamples), mInPos (-1), mOutPos (-1)
		{
			if (mDelaySamples > 0)
				mDelayBuffer.resize (maxSamplesPerBlock + mDelaySamples);
		}

		bool hasDelay () const { return mDelaySamples > 0; }
		int32 getBufferSamples () const { return mDelayBuffer.getMaxSamples (); }

		bool process (T* src, T* dst, int32 numSamples, bool silentIn)
		{
			bool silentOut = false;

			if (hasDelay () && src)
			{
				int32 bufferSize = getBufferSamples ();
				delay<T> (numSamples, src, dst, mDelayBuffer, bufferSize, mInPos, mOutPos);

				// update inPos, outPos
				mInPos += numSamples;
				if (mInPos >= bufferSize)
					mInPos -= bufferSize;
				mOutPos += numSamples;
				if (mOutPos >= bufferSize)
					mOutPos -= bufferSize;
			}
			else
			{
				if (src != dst)
				{
					if (src && !silentIn)
					{
						memcpy (dst, src, numSamples * sizeof (T));
					}
					else
					{
						memset (dst, 0, numSamples * sizeof (T));
						silentOut = true;
					}
				}
				else
				{
					silentOut = silentIn;
				}
			}
			return silentOut;
		}

		void flush ()
		{
			mDelayBuffer.clearAll ();

			mInPos = mOutPos = 0;
			if (hasDelay ())
				mOutPos = getBufferSamples () - mDelaySamples; // must be != inPos
		}
	};

	Delay* mDelays[kMaxChannelsSupported];
	
	bool mActive;
	bool mMainIOBypass;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
