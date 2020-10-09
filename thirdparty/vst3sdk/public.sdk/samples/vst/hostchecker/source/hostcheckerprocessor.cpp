//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/hostcheckerprocessor.cpp
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

#include "hostcheckerprocessor.h"
#include "cids.h"
#include "hostcheckercontroller.h"

#include "public.sdk/source/vst/vstaudioprocessoralgo.h"
#include "public.sdk/source/vst/vsteventshelper.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstpluginterfacesupport.h"

namespace Steinberg {
namespace Vst {

#define THREAD_CHECK_MSG(msg) "The host called '" msg "' in the wrong thread context.\n"

bool THREAD_CHECK_EXIT = false;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
HostCheckerProcessor::HostCheckerProcessor ()
{
	mCurrentState = State::kUninitialized;

	mLatency = 256;

	setControllerClass (HostCheckerControllerUID);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::initialize (FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result == kResultOk)
	{
		if (mCurrentState != State::kUninitialized)
			addLogEvent (kLogIdInvalidStateInitializedMissing);

		mCurrentState = State::kInitialized;

		addAudioInput (USTRING ("Audio Input"), SpeakerArr::kStereo);
		addAudioInput (USTRING ("Aux Input 1"), SpeakerArr::kStereo, kAux, 0);
		FUnknownPtr<IVst3ToAAXWrapper> AAXContext (context);
		if (AAXContext == nullptr)
		{
			addAudioInput (USTRING ("Aux Input 2"), SpeakerArr::kMono, kAux, 0);
			addAudioInput (USTRING ("Aux Input 3"), SpeakerArr::kMono, kAux, 0);
			addAudioInput (USTRING ("Aux Input 4"), SpeakerArr::kMono, kAux, 0);
			addAudioInput (USTRING ("Aux Input 5"), SpeakerArr::kMono, kAux, 0);
			addAudioInput (USTRING ("Aux Input 6"), SpeakerArr::kMono, kAux, 0);
			addAudioInput (USTRING ("Aux Input 7"), SpeakerArr::kMono, kAux, 0);
			addAudioInput (USTRING ("Aux Input 8"), SpeakerArr::kMono, kAux, 0);
			addAudioInput (USTRING ("Aux Input 9"), SpeakerArr::kMono, kAux, 0);
			addAudioInput (USTRING ("Aux Input 10"), SpeakerArr::kMono, kAux, 0);
		}

		addAudioOutput (USTRING ("Audio Output"), SpeakerArr::kStereo);

		addEventInput (USTRING ("Event Input 1"), 1);
		addEventInput (USTRING ("Event Input 2"), 1);

		addEventOutput (USTRING ("Event Output 1"), 1);
		addEventOutput (USTRING ("Event Output 2"), 1);

		mHostCheck.setComponent (this);
	}

	FUnknownPtr<IPlugInterfaceSupport> plugInterfaceSupport (context);
	if (plugInterfaceSupport)
	{
		addLogEvent (kLogIdIPlugInterfaceSupportSupported);

		if (plugInterfaceSupport->isPlugInterfaceSupported (IAudioPresentationLatency::iid) ==
		    kResultTrue)
			addLogEvent (kLogIdAudioPresentationLatencySamplesSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (IPrefetchableSupport::iid) ==
		    kResultTrue)
			addLogEvent (kLogIdIPrefetchableSupportSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (IProcessContextRequirements::iid) ==
		    kResultTrue)
			addLogEvent (kLogIdIProcessContextRequirementsSupported);
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::terminate ()
{
	if (mCurrentState == State::kUninitialized)
	{
		// redundance
	}
	else if (mCurrentState != State::kSetupDone)
	{
		// wrong state
	}
	mCurrentState = State::kUninitialized;
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
void HostCheckerProcessor::addLogEvent (Steinberg::int32 logId)
{
	mHostCheck.getEventLogger ().addLogEvent (logId);
}

//-----------------------------------------------------------------------------
void HostCheckerProcessor::addLogEventMessage (const LogEvent& logEvent)
{
	auto* evt = NEW LogEvent (logEvent);
	{
		Base::Thread::FGuard guard (msgQueueLock);
		msgQueue.push_back (evt);
	}
}

//-----------------------------------------------------------------------------
void HostCheckerProcessor::sendNowAllLogEvents ()
{
	const EventLogger::Codes& errors = mHostCheck.getEventLogs ();
	auto iter = errors.begin ();
	while (iter != errors.end ())
	{
		if ((*iter).fromProcessor && (*iter).count > 0)
		{
			sendLogEventMessage ((*iter));
		}
		++iter;
	}
	mHostCheck.getEventLogger ().resetLogEvents ();
}

//-----------------------------------------------------------------------------
void HostCheckerProcessor::sendLogEventMessage (const LogEvent& logEvent)
{
	if (auto message = owned (allocateMessage ()))
	{
		message->setMessageID ("LogEvent");
		IAttributeList* attributes = message->getAttributes ();
		if (attributes)
		{
			SMTG_ASSERT (logEvent.id >= 0);
			attributes->setInt ("ID", logEvent.id);
			attributes->setInt ("Count", logEvent.count);
			sendMessage (message);
		}
	}
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::setAudioPresentationLatencySamples (
    BusDirection dir, int32 busIndex, uint32 latencyInSamples)
{
	addLogEvent (kLogIdAudioPresentationLatencySamplesSupported);
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::getPrefetchableSupport (PrefetchableSupport& prefetchable)
{
	addLogEvent (kLogIdIPrefetchableSupportSupported);
	prefetchable = kIsYetPrefetchable;
	return kResultTrue;
}

//-----------------------------------------------------------------------------
uint32 PLUGIN_API HostCheckerProcessor::getProcessContextRequirements ()
{
	addLogEvent (kLogIdIProcessContextRequirementsSupported);

	processContextRequirements.needSystemTime ();
	processContextRequirements.needContinousTimeSamples ();
	processContextRequirements.needProjectTimeMusic ();
	processContextRequirements.needBarPositionMusic ();
	processContextRequirements.needCycleMusic ();
	processContextRequirements.needSamplesToNextClock ();
	processContextRequirements.needTempo ();
	processContextRequirements.needTimeSignature ();
	processContextRequirements.needChord ();
	processContextRequirements.needFrameRate ();
	processContextRequirements.needTransportState ();

	return AudioEffect::getProcessContextRequirements ();
}

//-----------------------------------------------------------------------------
void HostCheckerProcessor::informLatencyChanged ()
{
	auto* evt = NEW LogEvent;
	evt->id = kLogIdInformLatencyChanged;
	{
		Base::Thread::FGuard guard (msgQueueLock);
		msgQueue.push_back (evt);
	}
}

//-----------------------------------------------------------------------------
void HostCheckerProcessor::sendLatencyChanged ()
{
	if (IMessage* newMsg = allocateMessage ())
	{
		newMsg->setMessageID ("Latency");
		if (auto* attr = newMsg->getAttributes ())
			attr->setFloat ("Value", mWantedLatency);
		sendMessage (newMsg);
	}
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::process (ProcessData& data)
{
	mHostCheck.validate (data, mMinimumOfInputBufferCount, mMinimumOfOutputBufferCount);

	if (mCurrentState != State::kProcessing)
	{
		addLogEvent (kLogIdInvalidStateProcessingMissing);
	}
	if (mSetActiveCalled)
	{
		mSetActiveCalled = false;
		addLogEvent (kLogIdSetActiveCalledSupported);
	}
	// flush parameters case
	if (data.numInputs == 0 && data.numOutputs == 0)
	{
		addLogEvent (kLogIdParametersFlushSupported);
	}
	if (data.processContext)
	{
		if (data.processContext->state & ProcessContext::kPlaying)
			addLogEvent (kLogIdProcessContextPlayingSupported);
		if (data.processContext->state & ProcessContext::kRecording)
			addLogEvent (kLogIdProcessContextRecordingSupported);
		if (data.processContext->state & ProcessContext::kCycleActive)
			addLogEvent (kLogIdProcessContextCycleActiveSupported);

		if (data.processContext->state & ProcessContext::kSystemTimeValid)
			addLogEvent (kLogIdProcessContextSystemTimeSupported);
		if (data.processContext->state & ProcessContext::kContTimeValid)
			addLogEvent (kLogIdProcessContextContTimeSupported);
		if (data.processContext->state & ProcessContext::kProjectTimeMusicValid)
			addLogEvent (kLogIdProcessContextTimeMusicSupported);
		if (data.processContext->state & ProcessContext::kBarPositionValid)
			addLogEvent (kLogIdProcessContextBarPositionSupported);
		if (data.processContext->state & ProcessContext::kCycleValid)
			addLogEvent (kLogIdProcessContextCycleSupported);
		if (data.processContext->state & ProcessContext::kTempoValid)
			addLogEvent (kLogIdProcessContextTempoSupported);
		if (data.processContext->state & ProcessContext::kTimeSigValid)
			addLogEvent (kLogIdProcessContextTimeSigSupported);
		if (data.processContext->state & ProcessContext::kChordValid)
			addLogEvent (kLogIdProcessContextChordSupported);

		if (data.processContext->state & ProcessContext::kSmpteValid)
			addLogEvent (kLogIdProcessContextSmpteSupported);
		if (data.processContext->state & ProcessContext::kClockValid)
			addLogEvent (kLogIdProcessContextClockSupported);
	}

	Algo::foreach (data.inputParameterChanges, [&] (IParamValueQueue& paramQueue) {
		Algo::foreachLast (paramQueue, [&] (ParamID id, int32 sampleOffset, ParamValue value) {
			if (id == kBypassTag)
			{
				mBypass = value > 0;
				mBypassProcessorFloat.setActive (mBypass);
				mBypassProcessorDouble.setActive (mBypass);
			}
			else if (id == kLatencyTag)
			{
				mWantedLatency = value * HostChecker::kMaxLatency;
				addLogEvent (kLogIdInformLatencyChanged);
			}
			else if (id == kParam1Tag)
			{
			}
			else if (id == kGeneratePeaksTag)
			{
				mGeneratePeaks = value;
			}
		});
	});

	if (mBypassProcessorFloat.isActive ())
	{
		if (data.symbolicSampleSize == kSample32)
			mBypassProcessorFloat.process (data);
		else // kSample64
			mBypassProcessorDouble.process (data);
	}
	else if (data.numSamples && data.numOutputs)
	{
		if (data.numInputs > 0 && data.inputs[0].silenceFlags != 0)
		{
			addLogEvent (kLogIdSilentFlagsSupported);
		}
		if (data.numInputs > 1 && data.inputs[1].silenceFlags != 0)
		{
			addLogEvent (kLogIdSilentFlagsSCSupported);
		}

		// Generate output (peak at a given tempo) (overwrite the input)
		if (mGeneratePeaks > 0 && data.processContext)
		{
			if (data.symbolicSampleSize == kSample32)
				Algo::clear32 (data.outputs, data.numSamples);
			else // kSample64
				Algo::clear64 (data.outputs, data.numSamples);

			float coef = mGeneratePeaks * mLastBlockMarkerValue;

			float distance2BarPosition =
			    (data.processContext->projectTimeMusic - data.processContext->barPositionMusic) /
			    (4. * data.processContext->timeSigNumerator) *
			    data.processContext->timeSigDenominator / 2.;

			// Normalized Tempo [0, 360] => [0, 1]
			float tempo = data.processContext->tempo / 360.;

			if (data.symbolicSampleSize == kSample32)
			{
				for (int32 i = 0; i < data.outputs[0].numChannels && i < 1; i++)
				{
					data.outputs[0].channelBuffers32[i][0] = coef;
					if (data.processContext->state & ProcessContext::kTempoValid &&
					    data.numSamples > 3)
						data.outputs[0].channelBuffers32[i][3] = tempo;
				}
				if (data.processContext->state & ProcessContext::kBarPositionValid)
				{
					for (int32 i = 1; i < data.outputs[0].numChannels; i++)
					{
						data.outputs[0].channelBuffers32[i][0] = distance2BarPosition;
					}
				}
			}
			else // kSample64
			{
				for (int32 i = 0; i < data.outputs[0].numChannels && i < 1; i++)
				{
					data.outputs[0].channelBuffers64[i][0] = coef;
					if (data.processContext->state & ProcessContext::kTempoValid &&
					    data.numSamples > 3)
						data.outputs[0].channelBuffers64[i][3] = tempo;
				}
				if (data.processContext->state & ProcessContext::kBarPositionValid)
				{
					for (int32 i = 1; i < data.outputs[0].numChannels; i++)
					{
						data.outputs[0].channelBuffers64[i][0] = distance2BarPosition;
					}
				}
			}
			// mLastBlockMarkerValue = -mLastBlockMarkerValue;

			data.outputs[0].silenceFlags = 0x0;

			const float kMaxNotesToDisplay = 5.f;

			// check event from all input events and send them to the output event
			Algo::foreach (data.inputEvents, [&] (Event& event) {
				switch (event.type)
				{
					case Event::kNoteOnEvent:
						mNumNoteOns++;
						if (data.symbolicSampleSize == kSample32)
							data.outputs[0].channelBuffers32[0][event.sampleOffset] =
							    mNumNoteOns / kMaxNotesToDisplay;
						else // kSample64
							data.outputs[0].channelBuffers64[0][event.sampleOffset] =
							    mNumNoteOns / kMaxNotesToDisplay;
						if (data.outputEvents)
						{
							data.outputEvents->addEvent (event);

							Event evtMIDICC;
							Helpers::initLegacyMIDICCOutEvent (evtMIDICC, kCtrlModWheel,
							                                   event.noteOn.channel,
							                                   event.noteOn.velocity);
							data.outputEvents->addEvent (evtMIDICC);
						}
						break;
					case Event::kNoteOffEvent:
						if (data.symbolicSampleSize == kSample32)
							data.outputs[0].channelBuffers32[1][event.sampleOffset] =
							    -mNumNoteOns / kMaxNotesToDisplay;
						else // kSample64
							data.outputs[0].channelBuffers64[1][event.sampleOffset] =
							    -mNumNoteOns / kMaxNotesToDisplay;
						if (data.outputEvents)
							data.outputEvents->addEvent (event);
						mNumNoteOns--;
						break;
					default: break;
				}
			});
		}
		else
		{
			//---get audio buffers----------------
			uint32 sampleFramesSize = getSampleFramesSizeInBytes (processSetup, data.numSamples);
			void** in = getChannelBuffersPointer (processSetup, data.inputs[0]);
			void** out = getChannelBuffersPointer (processSetup, data.outputs[0]);

			for (int32 i = 0; i < data.outputs[0].numChannels && i < data.inputs[0].numChannels;
			     i++)
			{
				// do not need to be copied if the buffers are the same
				if (in[i] != out[i])
				{
					memcpy (out[i], in[i], sampleFramesSize);
				}
			}

			data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
		}
	}

	if (data.outputParameterChanges)
	{
		const EventLogger::Codes& errors = mHostCheck.getEventLogs ();
		auto iter = errors.begin ();

		uint32 warnIdValue[HostChecker::kParamWarnCount] = {0};
		while (iter != errors.end ())
		{
			if ((*iter).fromProcessor && (*iter).count > 0)
			{
				int64 id = (*iter).id;
				int32 offset = id / HostChecker::kParamWarnBitCount;
				id = id % HostChecker::kParamWarnBitCount;
				warnIdValue[offset] |= 1 << id;
				// addLogEventMessage ((*iter));
			}
			++iter;
		}
		for (uint32 i = 0; i < HostChecker::kParamWarnCount; i++)
		{
			if (warnIdValue[i] != 0)
			{
				int32 idx;
				if (auto* queue =
				        data.outputParameterChanges->addParameterData (kProcessWarnTag + i, idx))
					queue->addPoint (
					    0, (double)warnIdValue[i] / double (HostChecker::kParamWarnStepCount), idx);
			}
		}
		mHostCheck.getEventLogger ().resetLogEvents ();
	}
	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::setupProcessing (ProcessSetup& setup)
{
	if (mCurrentState != State::kInitialized && mCurrentState != State::kSetupDone)
	{
		addLogEvent (kLogIdInvalidStateInitializedMissing);
	}

	mCurrentState = State::kSetupDone;

	mHostCheck.setProcessSetup (setup);
	return AudioEffect::setupProcessing (setup);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::setActive (TBool state)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerProcessor::setActive"),
	                          THREAD_CHECK_EXIT))
	{
		addLogEvent (kLogIdSetActiveCalledinWrongThread);
	}

	// we should not be in kActivated State!
	if (mCurrentState == State::kProcessing)
	{
		addLogEvent (kLogIdInvalidStateSetActiveWrong);
	}

	if (!state)
	{
		// only possible previous State: kActivated
		if (mCurrentState == State::kSetupDone)
		{
			addLogEvent (kLogIdsetActiveFalseRedundant);
		}

		mCurrentState = State::kSetupDone;
		mBypassProcessorFloat.reset ();
		mBypassProcessorDouble.reset ();
	}
	else
	{
		mSetActiveCalled = true;

		// only possible previous State: kSetupDone
		if (mCurrentState == State::kActivated)
		{
			addLogEvent (kLogIdsetActiveTrueRedundant);
		}
		else if (mCurrentState != State::kSetupDone)
		{
			addLogEvent (kLogIdInvalidStateSetupMissing);
		}

		mCurrentState = State::kActivated;
		mLatency = mWantedLatency;

		// prepare bypass
		mBypassProcessorFloat.setup (*this, processSetup, mLatency);
		mBypassProcessorDouble.setup (*this, processSetup, mLatency);
	}
	mLastBlockMarkerValue = -0.5f;
	mNumNoteOns = 0;

	sendNowAllLogEvents ();

	return AudioEffect::setActive (state);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::notify (IMessage* message)
{
	if (!message)
		return kInvalidArgument;

	if (FIDStringsEqual (message->getMessageID (), "Parameter"))
	{
		int64 paramId = -1;
		if (message->getAttributes ()->getInt ("ID", paramId) == kResultOk)
		{
			mHostCheck.addParameter (paramId);
		}
	}

	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	if (symbolicSampleSize == kSample32)
	{
		addLogEvent (kLogIdCanProcessSampleSize32);
		return kResultTrue;
	}
	else if (symbolicSampleSize == kSample64)
	{
		addLogEvent (kLogIdCanProcessSampleSize64);
		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
uint32 PLUGIN_API HostCheckerProcessor::getLatencySamples ()
{
	addLogEvent (kLogIdGetLatencySamples);
	return mLatency;
}

//-----------------------------------------------------------------------------
uint32 PLUGIN_API HostCheckerProcessor::getTailSamples ()
{
	addLogEvent (kLogIdGetTailSamples);
	return mLatency;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::getRoutingInfo (RoutingInfo& inInfo, RoutingInfo& outInfo)
{
	addLogEvent (kLogIdGetRoutingInfo);
	return AudioEffect::getRoutingInfo (inInfo, outInfo);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::activateBus (MediaType type, BusDirection dir, int32 index,
                                                      TBool state)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerProcessor::activateBus"),
	                          THREAD_CHECK_EXIT))
	{
		addLogEvent (kLogIdactivateBusCalledinWrongThread);
	}

	if (type == kAudio && dir == kInput)
	{
		if (index < 0 || index >= static_cast<int32> (getBusList (kAudio, kInput)->size ()))
			addLogEvent (kLogIdInvalidActivateAuxBus);
		else if (index > 0)
			addLogEvent (kLogIdActivateAuxBus);
	}

	auto result = AudioEffect::activateBus (type, dir, index, state);

	if (result == kResultTrue && type == kAudio)
	{
		if (auto list = getBusList (type, dir))
		{
			int32 lastActive = -1;
			for (int32 idx = static_cast<int32> (list->size ()) - 1; idx >= 0; --idx)
			{
				if (list->at (idx)->isActive ())
				{
					lastActive = idx;
					break;
				}
			}
			if (dir == kInput)
				mMinimumOfInputBufferCount = lastActive + 1;
			else
				mMinimumOfOutputBufferCount = lastActive + 1;
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::setBusArrangements (SpeakerArrangement* inputs,
                                                             int32 numIns,
                                                             SpeakerArrangement* outputs,
                                                             int32 numOuts)
{
	addLogEvent (kLogIdSetBusArrangements);
	return AudioEffect::setBusArrangements (inputs, numIns, outputs, numOuts);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::getBusArrangement (BusDirection dir, int32 busIndex,
                                                            SpeakerArrangement& arr)
{
	addLogEvent (kLogIdGetBusArrangements);
	return AudioEffect::getBusArrangement (dir, busIndex, arr);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::setProcessing (TBool state)
{
	if (state)
	{
		// only possible previous State: kActivated
		if (mCurrentState != State::kActivated)
			addLogEvent (kLogIdInvalidStateSetProcessingWrong);

		if (mCurrentState == State::kProcessing)
			addLogEvent (kLogIdsetProcessingTrueRedundant);
		mCurrentState = State::kProcessing;
	}
	else
	{
		if (mCurrentState != State::kProcessing)
			addLogEvent (kLogIdsetProcessingFalseRedundant);
		mCurrentState = State::kActivated;
	}

	AudioEffect::setProcessing (state);
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::setState (IBStream* state)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerProcessor::setState"),
	                          THREAD_CHECK_EXIT))
	{
		addLogEvent (kLogIdProcessorSetStateCalledinWrongThread);
	}

	FUnknownPtr<IStreamAttributes> stream (state);

	if (IAttributeList* list = stream ? stream->getAttributes () : nullptr)
	{
		addLogEvent (kLogIdIAttributeListInSetStateSupported);
	}

	IBStreamer streamer (state, kLittleEndian);

	float saved = 0.f;
	if (streamer.readFloat (saved) == false)
		return kResultFalse;
	if (saved != 12345.67f)
	{
		SMTG_ASSERT (false)
	}

	uint32 latency = mLatency;
	if (streamer.readInt32u (latency) == false)
		return kResultFalse;

	uint32 bypass;
	if (streamer.readInt32u (bypass) == false)
		return kResultFalse;
	mBypass = bypass > 0;
	mBypassProcessorFloat.setActive (mBypass);
	mBypassProcessorDouble.setActive (mBypass);

	if (latency != mLatency)
	{
		mLatency = latency;
		sendLatencyChanged ();
	}

	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerProcessor::getState (IBStream* state)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerProcessor::getState"),
	                          THREAD_CHECK_EXIT))
	{
		addLogEvent (kLogIdProcessorGetStateCalledinWrongThread);
	}

	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	float toSave = 12345.67f;
	streamer.writeFloat (toSave);
	streamer.writeInt32u (mLatency);
	streamer.writeInt32u (mBypass ? 1 : 0);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
