//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/hostcheckerprocessor.h
// Created by  : Steinberg, 04/2012
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

#include "hostcheck.h"
#include "logevents.h"

#include "public.sdk/source/common/threadchecker.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "public.sdk/source/vst/vstbypassprocessor.h"
#include "base/thread/include/flock.h"
#include "pluginterfaces/vst/ivstprefetchablesupport.h"

#include <list>

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
class HostCheckerProcessor : public AudioEffect,
                             public IAudioPresentationLatency,
                             public IPrefetchableSupport
{
public:
	HostCheckerProcessor ();

	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API terminate () SMTG_OVERRIDE;

	tresult PLUGIN_API process (ProcessData& data) SMTG_OVERRIDE;
	tresult PLUGIN_API setupProcessing (ProcessSetup& setup) SMTG_OVERRIDE;
	tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;
	tresult PLUGIN_API notify (IMessage* message) SMTG_OVERRIDE;
	uint32 PLUGIN_API getLatencySamples () SMTG_OVERRIDE;
	uint32 PLUGIN_API getTailSamples () SMTG_OVERRIDE;
	tresult PLUGIN_API canProcessSampleSize (int32 symbolicSampleSize) SMTG_OVERRIDE;
	tresult PLUGIN_API setProcessing (TBool state) SMTG_OVERRIDE;

	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;

	tresult PLUGIN_API getRoutingInfo (RoutingInfo& inInfo, RoutingInfo& outInfo) SMTG_OVERRIDE;
	tresult PLUGIN_API activateBus (MediaType type, BusDirection dir, int32 index,
	                                TBool state) SMTG_OVERRIDE;
	tresult PLUGIN_API setBusArrangements (SpeakerArrangement* inputs, int32 numIns,
	                                       SpeakerArrangement* outputs,
	                                       int32 numOuts) SMTG_OVERRIDE;
	tresult PLUGIN_API getBusArrangement (BusDirection dir, int32 busIndex,
	                                      SpeakerArrangement& arr) SMTG_OVERRIDE;

	static FUnknown* createInstance (void*)
	{
		return (IAudioProcessor*)new HostCheckerProcessor ();
	}

	//---IAudioPresentationLatency-------------------------
	tresult PLUGIN_API setAudioPresentationLatencySamples (BusDirection dir, int32 busIndex,
	                                                       uint32 latencyInSamples) SMTG_OVERRIDE;

	//---IPrefetchableSupport------------------------------
	tresult PLUGIN_API getPrefetchableSupport (PrefetchableSupport& prefetchable /*out*/)
	    SMTG_OVERRIDE;

	//---IProcessContextRequirements-----------------------
	uint32 PLUGIN_API getProcessContextRequirements () SMTG_OVERRIDE;

	DEFINE_INTERFACES
		DEF_INTERFACE (IAudioPresentationLatency)
		DEF_INTERFACE (IPrefetchableSupport)
	END_DEFINE_INTERFACES (AudioEffect)
	REFCOUNT_METHODS (AudioEffect)

	enum class State : uint32
	{
		kUninitialized = 0,
		kInitialized,
		kSetupDone,
		kActivated,
		kProcessing
	};

protected:
	void addLogEvent (Steinberg::int32 logId);

	void informLatencyChanged ();
	void sendLatencyChanged ();

	void addLogEventMessage (const LogEvent& logEvent);
	void sendLogEventMessage (const LogEvent& logEvent);
	void sendNowAllLogEvents ();

	HostCheck mHostCheck;

	BypassProcessor<Vst::Sample32> mBypassProcessorFloat;
	BypassProcessor<Vst::Sample64> mBypassProcessorDouble;

	float mLastBlockMarkerValue = -0.5f;

	int32 mNumNoteOns = 0;
	uint32 mLatency = 0;
	uint32 mWantedLatency = 0;
	float mGeneratePeaks = 0;
	State mCurrentState {State::kUninitialized};

	uint32 mMinimumOfInputBufferCount {0};
	uint32 mMinimumOfOutputBufferCount {0};

	std::unique_ptr<ThreadChecker> threadChecker {ThreadChecker::create ()};

	Steinberg::Base::Thread::FLock msgQueueLock;
	std::list<LogEvent*> msgQueue;

	bool mBypass {false};
	bool mSetActiveCalled {false};
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
