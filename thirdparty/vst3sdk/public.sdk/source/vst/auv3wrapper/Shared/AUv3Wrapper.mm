//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : AUv3Wrapper.mm
// Created by  : Steinberg, 07/2017.
// Description : VST 3 AUv3Wrapper
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

#import "AUv3Wrapper.h"

#import <AVFoundation/AVFoundation.h>

#import "public.sdk/source/vst/auwrapper/NSDataIBStream.h"
#import "public.sdk/source/vst/hosting/eventlist.h"
#import "public.sdk/source/vst/hosting/parameterchanges.h"
#import "public.sdk/source/vst/hosting/processdata.h"
#import "public.sdk/source/vst/utility/mpeprocessor.h"
#import "public.sdk/source/vst/vsteditcontroller.h"
#import "base/source/timer.h"
#import "pluginterfaces/base/ustring.h"
#import "pluginterfaces/gui/iplugview.h"
#import "pluginterfaces/vst/ivstmidicontrollers.h"
#import "pluginterfaces/vst/ivstphysicalui.h"
#import "pluginterfaces/vst/ivstpluginterfacesupport.h"
#import "pluginterfaces/vst/vstpresetkeys.h"
#import "pluginterfaces/vst/vsttypes.h"

#import <array>
#import <atomic>

#if TARGET_OS_IPHONE
#define SMTG_IOS_MAC_PLATFORMTYPE kPlatformTypeUIView
#define SMTG_IOS_MAC_VIEW UIView
#else
#define SMTG_IOS_MAC_VIEW NSView
#define SMTG_IOS_MAC_PLATFORMTYPE kPlatformTypeNSView
#endif

extern "C" {

//------------------------------------------------------------------------
#if SMTG_AUV3_WRAPPER_EXTERNAL_PLUGIN_FACTORY
// PluginFactory will be provided externally
Steinberg::IPluginFactory* PLUGIN_API GetPluginFactory ();

#else
// Generate our own factory wrapping the VST3 variant
struct VSTBundle
{
	using EntryPtr = bool (*) (CFBundleRef);
	using ExitPtr = bool (*) (void);

	VSTBundle ()
	{
		initialized = init ();
	}
	
	bool loadBundle (CFURLRef bundleUrl)
	{
		bundle = CFBundleCreate (kCFAllocatorDefault, bundleUrl);
		if (!bundle)
			return false;
		if (!CFBundleLoadExecutable (bundle))
			return false;
		bundleEntry = (EntryPtr)CFBundleGetFunctionPointerForName (bundle, CFSTR ("bundleEntry"));
		if (!bundleEntry)
			return false;
		bundleExit = (ExitPtr)CFBundleGetFunctionPointerForName (bundle, CFSTR ("bundleExit"));
		if (!bundleExit)
			return false;
		getFactory =
			(GetFactoryProc)CFBundleGetFunctionPointerForName (bundle, CFSTR ("GetPluginFactory"));
		if (!bundleExit)
			return false;

		if (!bundleEntry (bundle))
			return false;
		return true;
	}
	
	bool init ()
	{
		NSURL* url = NSBundle.mainBundle.builtInPlugInsURL;
		url = [url URLByAppendingPathComponent:@"plugin.vst3"];
		if (!url)
			return false;
		CFURLRef bundleUrl = (__bridge CFURLRef)url;
		if (!loadBundle (bundleUrl))
		{
			url = NSBundle.mainBundle.builtInPlugInsURL;
			url = [url URLByAppendingPathComponent:@"auv3.appex"];
			NSBundle* appexBundle = [NSBundle bundleWithURL:url];
			if (!appexBundle)
				return false;
			url = appexBundle.builtInPlugInsURL;
			url = [url URLByAppendingPathComponent:@"plugin.vst3"];
			if (!url)
				return false;
			bundleUrl = (__bridge CFURLRef)url;
			if (!loadBundle (bundleUrl))
				return false;
		}

		return true;
	}

	bool initialized {false};
	CFBundleRef bundle {nullptr};
	EntryPtr bundleEntry {nullptr};
	ExitPtr bundleExit {nullptr};
	GetFactoryProc getFactory {nullptr};
};

//------------------------------------------------------------------------
Steinberg::IPluginFactory* PLUGIN_API GetPluginFactory ()
{
	static VSTBundle vstBundle;
	if (vstBundle.initialized)
	{
		return vstBundle.getFactory ();
	}
	return nullptr;
}
#endif // SMTG_AUV3_WRAPPER_CUSTOM_PLUGIN_FACTORY

} // extern "C"

#pragma mark - Helpers from AUv2Wrapper / aucocoaview
//------------------------------------------------------------------------
namespace Steinberg {

class AUPlugFrame : public FObject, public IPlugFrame
{
public:
	AUPlugFrame (SMTG_IOS_MAC_VIEW* parent) : parent (parent) {}
	tresult PLUGIN_API resizeView (IPlugView* view, ViewRect* vr) SMTG_OVERRIDE
	{
		CGRect newSize = CGRectMake ([parent frame].origin.x, [parent frame].origin.y,
		                             vr->right - vr->left, vr->bottom - vr->top);
		[parent setFrame:newSize];
		return kResultTrue;
	}

	OBJ_METHODS (AUPlugFrame, FObject)
	DEF_INTERFACES_1 (IPlugFrame, FObject)
	REFCOUNT_METHODS (FObject)

protected:
	SMTG_IOS_MAC_VIEW* parent;
};

namespace Vst {

//------------------------------------------------------------------------
static CFStringRef createCFStringFromString128 (const String128& string)
{
	UString128 str (string);
	return CFStringCreateWithCharacters (0, (const UniChar*)string, str.getLength ());
}

//------------------------------------------------------------------------
static SpeakerArrangement numChannelsToSpeakerArrangement (UInt32 numChannels)
{
	switch (numChannels)
	{
		case 1: return SpeakerArr::kMono;
		case 2: return SpeakerArr::kStereo;
		case 6: return SpeakerArr::k51;
	}
	return 0;
}

//------------------------------------------------------------------------
class AUHostApplication : public FObject,
                          public HostApplication,
                          public IVst3ToAUWrapper,
                          public IVst3WrapperMPESupport
{
public:
	__weak AUv3Wrapper* wrapper {nil};

	AUHostApplication ()
	{
		auto pis = getPlugInterfaceSupport ();
		pis->addPlugInterfaceSupported (INoteExpressionController::iid);
		pis->addPlugInterfaceSupported (INoteExpressionPhysicalUIMapping::iid);
	}

	tresult PLUGIN_API getName (String128 name) SMTG_OVERRIDE
	{
		String str ("VST3-AU Wrapper");
		str.copyTo (name, 0, 127);
		return kResultTrue;
	}

	tresult PLUGIN_API enableMPEInputProcessing (TBool state) SMTG_OVERRIDE
	{
		if (!wrapper)
			return kInternalError;
		return [wrapper enableMPESupport:state != 0 ? YES : NO] ? kResultTrue : kResultFalse;
	}

	tresult PLUGIN_API setMPEInputDeviceSettings (int32 masterChannel, int32 memberBeginChannel,
	                                              int32 memberEndChannel) SMTG_OVERRIDE
	{
		if (!wrapper)
			return kInternalError;
		return [wrapper setMPEInputDeviceMasterChannel:masterChannel
		                            memberBeginChannel:memberBeginChannel
		                              memberEndChannel:memberEndChannel] ?
		           kResultTrue :
		           kResultFalse;
	}

	DEFINE_INTERFACES
		DEF_INTERFACE (IVst3ToAUWrapper)
		DEF_INTERFACE (IVst3WrapperMPESupport)
	END_DEFINE_INTERFACES (HostApplication)
	REFCOUNT_METHODS (HostApplication)
};

//------------------------------------------------------------------------
typedef std::map<UnitID, UnitInfo> UnitInfoMap;
typedef std::vector<String> ParameterGroupVector;

static void buildUnitInfos (IUnitInfo* unitInfoController, UnitInfoMap& units)
{
	units.clear ();
	if (unitInfoController)
	{
		int32 numUnits = unitInfoController->getUnitCount ();
		for (int32 i = 0; i < numUnits; i++)
		{
			UnitInfo ui;
			if (unitInfoController->getUnitInfo (i, ui) == kResultTrue)
				units[ui.id] = ui;
		}
	}
}

//------------------------------------------------------------------------
struct BufferedAudioBus
{
	AUAudioUnitBus* bus = nullptr;
	AUAudioFrameCount maxFrames = 0;
	AVAudioPCMBuffer* pcmBuffer = nullptr;
	AudioBufferList const* originalAudioBufferList = nullptr;
	AudioBufferList* mutableAudioBufferList = nullptr;

	void init (AVAudioFormat* defaultFormat, AVAudioChannelCount maxChannels)
	{
		maxFrames = 0;
		pcmBuffer = nullptr;
		originalAudioBufferList = nullptr;
		mutableAudioBufferList = nullptr;

		bus = [[AUAudioUnitBus alloc] initWithFormat:defaultFormat error:nil];

		bus.maximumChannelCount = maxChannels;
	}

	void allocateRenderResources (AUAudioFrameCount inMaxFrames)
	{
		maxFrames = inMaxFrames;

		pcmBuffer = [[AVAudioPCMBuffer alloc] initWithPCMFormat:bus.format frameCapacity:maxFrames];

		originalAudioBufferList = pcmBuffer.audioBufferList;
		mutableAudioBufferList = pcmBuffer.mutableAudioBufferList;
	}

	void deallocateRenderResources ()
	{
		pcmBuffer = nullptr;
		originalAudioBufferList = nullptr;
		mutableAudioBufferList = nullptr;
	}
};

//------------------------------------------------------------------------
// This struct provides a prepareOutputBufferList method to copy the internal buffer pointers to the
// output buffer list in case the client passed in null buffer pointers.
struct BufferedOutputBus : BufferedAudioBus
{
	void prepareOutputBufferList (AudioBufferList* outBufferList, AVAudioFrameCount frameCount,
	                              bool zeroFill)
	{
		UInt32 byteSize = frameCount * sizeof (float);

		for (UInt32 i = 0; i < outBufferList->mNumberBuffers; ++i)
		{
			outBufferList->mBuffers[i].mNumberChannels =
			    originalAudioBufferList->mBuffers[i].mNumberChannels;
			outBufferList->mBuffers[i].mDataByteSize = byteSize;

			if (outBufferList->mBuffers[i].mData == nullptr)
				outBufferList->mBuffers[i].mData = originalAudioBufferList->mBuffers[i].mData;

			if (zeroFill)
				memset (outBufferList->mBuffers[i].mData, 0, byteSize);
		}
	}
};

//------------------------------------------------------------------------
// This struct manages a buffer into which an audio unit with input busses can pull its input data.
struct BufferedInputBus : BufferedAudioBus
{
	AUAudioUnitStatus pullInput (AudioUnitRenderActionFlags* actionFlags,
	                             AudioTimeStamp const* timestamp, AVAudioFrameCount frameCount,
	                             NSInteger inputBusNumber, AURenderPullInputBlock pullInputBlock)
	{
		if (pullInputBlock == nullptr)
			return kAudioUnitErr_NoConnection;

		prepareInputBufferList ();

		return pullInputBlock (actionFlags, timestamp, frameCount, inputBusNumber,
		                       mutableAudioBufferList);
	}

	void prepareInputBufferList ()
	{
		UInt32 byteSize = maxFrames * sizeof (float);

		mutableAudioBufferList->mNumberBuffers = originalAudioBufferList->mNumberBuffers;

		for (UInt32 i = 0; i < originalAudioBufferList->mNumberBuffers; ++i)
		{
			mutableAudioBufferList->mBuffers[i].mNumberChannels =
			    originalAudioBufferList->mBuffers[i].mNumberChannels;
			mutableAudioBufferList->mBuffers[i].mData = originalAudioBufferList->mBuffers[i].mData;
			mutableAudioBufferList->mBuffers[i].mDataByteSize = byteSize;
		}
	}
};

#pragma mark - ComponentHelper class (implementing IComponentHandler)
//------------------------------------------------------------------------
class ComponentHelper : public IComponentHandler
{
public:
	ComponentHelper (void* audioUnit);
	virtual ~ComponentHelper ();

	__weak AUv3Wrapper* auv3Wrapper;

	DECLARE_FUNKNOWN_METHODS

	tresult PLUGIN_API performEdit (ParamID tag, ParamValue valueNormalized) SMTG_OVERRIDE;

protected:
	tresult PLUGIN_API beginEdit (ParamID tag) SMTG_OVERRIDE;
	tresult PLUGIN_API endEdit (ParamID tag) SMTG_OVERRIDE;
	tresult PLUGIN_API restartComponent (int32 flags) SMTG_OVERRIDE;
};

//------------------------------------------------------------------------
ComponentHelper::ComponentHelper (void* audioUnit)
{
	FUNKNOWN_CTOR
	auv3Wrapper = (__bridge AUv3Wrapper*)audioUnit;
}

//------------------------------------------------------------------------
ComponentHelper::~ComponentHelper ()
{
	FUNKNOWN_DTOR
	auv3Wrapper = nil;
}

IMPLEMENT_FUNKNOWN_METHODS (ComponentHelper, IComponentHandler, IComponentHandler::iid)

//------------------------------------------------------------------------
tresult PLUGIN_API ComponentHelper::beginEdit (ParamID tag)
{
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ComponentHelper::performEdit (ParamID tag, ParamValue valueNormalized)
{
	[auv3Wrapper performEdit:tag value:valueNormalized];
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ComponentHelper::endEdit (ParamID tag)
{
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ComponentHelper::restartComponent (int32 flags)
{
	tresult result = kNotImplemented;
	// this method informs the host/the AU that something in the VST has changed (latency, parameter
	// titles, etc.)

	if (flags & kParamValuesChanged)
	{
		// vst plugin parameter names (titles) have changed
		[auv3Wrapper syncParameterValues];
		result = kResultTrue;
	}

	if (flags & kParamTitlesChanged)
	{
		// vst plugin parameter values have changed
		[auv3Wrapper initializeParameters];
		result = kResultTrue;
	}

	if (flags & kNoteExpressionChanged)
	{
		[auv3Wrapper onNoteExpressionChanged];
	}

	if (flags & kLatencyChanged)
	{
		result = kResultTrue;
	}

	// TODO: finish restartComponent implementation

	return result;
}

//------------------------------------------------------------------------
//  TimerHelper class
//------------------------------------------------------------------------
#pragma mark - TimerHelper class (implementing ITimerCallback)
class TimerHelper : public ITimerCallback
{
public:
	TimerHelper (void* audioUnit);
	~TimerHelper ();

	__weak AUv3Wrapper* auv3Wrapper;
	Timer* timer;

protected:
	void onTimer (Timer* timer);
};

//------------------------------------------------------------------------
TimerHelper::TimerHelper (void* audioUnit)
{
	auv3Wrapper = (__bridge AUv3Wrapper*)audioUnit;
	timer = Timer::create (this, 20);
}

//------------------------------------------------------------------------
TimerHelper::~TimerHelper ()
{
	auv3Wrapper = nil;
	timer->release ();
}

//------------------------------------------------------------------------
void TimerHelper::onTimer (Timer* timer)
{
	[auv3Wrapper onTimer];
}

#pragma mark - Render Helper structs
//------------------------------------------------------------------------
struct MPEHandler : public MPE::Handler
{
	struct Context
	{
		IEventList* inputEvents {nullptr};
		IMidiMapping* midiMapping {nullptr};
		IParameterChanges* outputParamChanges {nullptr};
		IComponentHandler* editPerformer {nullptr};
		MPE::Processor* mpeProcessor {nullptr};
		std::atomic_bool enableMPEProcessing {true};
		std::array<ParamID, 16> programChangeParameters {{kNoParamId}};
		std::array<ParamID, 16> programChangeParameterStepCount {{0}};
		std::array<NoteExpressionTypeID, 3> mpeMap {{kInvalidTypeID}};
	};

	void process (Context& c, const AUMIDIEvent* event, const AudioTimeStamp* timeStamp)
	{
		context = &c;
		currentSampleOffset = event->eventSampleTime - timeStamp->mSampleTime;
		if (c.enableMPEProcessing.load ())
		{
			mpeProcssingEnabled = true;
			c.mpeProcessor->processMIDIInput (event->data, event->length);
		}
		else
		{
			if (mpeProcssingEnabled)
			{
				mpeProcssingEnabled = false;
				c.mpeProcessor->reset ();
			}
			onOtherInput (event->data, event->length);
		}

		context = nullptr;
	}

private:
	Context* context {nullptr};
	int32 currentSampleOffset {0};
	MPE::NoteID noteIDCounter {0};
	bool mpeProcssingEnabled {false};
	using Pitch2ParamID = std::array<ParamID, 128>;
	std::array<Pitch2ParamID, 16> pitchToParamIDMap {{{0}}};

	bool generateNewNoteID (MPE::NoteID& outNoteID) override
	{
		if (!context)
			return false;
		outNoteID = ++noteIDCounter;
		return true;
	}
	void releaseNoteID (MPE::NoteID noteID) override {}

	void onMPENoteOn (MPE::NoteID noteID, MPE::Pitch pitch, MPE::Velocity velocity) override
	{
		if (!context)
			return;
		sendNoteOn (noteID, pitch, velocity);
	}

	void onMPENoteOff (MPE::NoteID noteID, MPE::Pitch pitch, MPE::Velocity velocity) override
	{
		if (!context)
			return;
		sendNoteOff (noteID, pitch, velocity);
	}

	void onMPEControllerChange (MPE::NoteID noteID, MPE::Controller cc,
	                            MPE::NormalizedValue value) override
	{
		if (!context || cc > MPE::Controller::Y)
			return;
		if (context->mpeMap[static_cast<size_t> (cc)] != kInvalidTypeID)
		{
			Event e {};
			e.type = Event::kNoteExpressionValueEvent;
			e.noteExpressionValue.noteId = noteID;
			e.noteExpressionValue.typeId = context->mpeMap[static_cast<size_t> (cc)];
			e.noteExpressionValue.value = value;
			e.sampleOffset = currentSampleOffset;
			context->inputEvents->addEvent (e);
		}
	}

	void sendNoteOn (MPE::NoteID noteID, MPE::Pitch pitch, MPE::Velocity velocity,
	                 int16 channel = 0)
	{
		if (!context->inputEvents)
			return;
		Event e {};
		e.type = Event::kNoteOnEvent;
		e.noteOn.pitch = pitch;
		e.noteOn.velocity = velocity;
		e.noteOn.noteId = noteID;
		e.noteOn.channel = channel;
		e.sampleOffset = currentSampleOffset;
		context->inputEvents->addEvent (e);
	}

	void sendNoteOff (MPE::NoteID noteID, MPE::Pitch pitch, MPE::Velocity velocity,
	                  int16 channel = 0)
	{
		if (!context->inputEvents)
			return;
		Event e {};
		e.type = Event::kNoteOffEvent;
		e.noteOff.pitch = pitch;
		e.noteOff.velocity = velocity;
		e.noteOff.noteId = noteID;
		e.noteOff.channel = channel;
		e.sampleOffset = currentSampleOffset;
		context->inputEvents->addEvent (e);
	}

	void sendPolyPressure (MPE::NoteID noteID, MPE::Pitch pitch, MPE::NormalizedValue pressure,
	                       int16 channel)
	{
		if (!context->inputEvents)
			return;
		Event e = {0};
		e.type = Event::kPolyPressureEvent;
		e.polyPressure.channel = channel;
		e.polyPressure.pitch = pitch;
		e.polyPressure.pressure = pressure;
		e.polyPressure.noteId = noteID;
		e.sampleOffset = currentSampleOffset;
		context->inputEvents->addEvent (e);
	}

	void sendControllerChange (CtrlNumber controller, MPE::NormalizedValue value, int16 channel)
	{
		if (!context->midiMapping)
			return;
		ParamID outParamID;
		if (context->midiMapping->getMidiControllerAssignment (0, channel, controller,
		                                                       outParamID) != kResultTrue)
			return;
		context->editPerformer->performEdit (outParamID, value);
		int32 index;
		if (auto vq = context->outputParamChanges->addParameterData (outParamID, index))
			vq->addPoint (currentSampleOffset, value, index);
	}

	void sendProgramChange (int16 channel, uint8 midiValue)
	{
		auto prgChangeParamID = context->programChangeParameters[channel];
		if (prgChangeParamID == kNoParamId)
			return;
		auto value = -1.;
		auto stepCount = context->programChangeParameterStepCount[channel];
		if (stepCount = 0)
			value = static_cast<double> (midiValue) / 127.;
		else if (midiValue <= stepCount)
			value = static_cast<double> (midiValue) / static_cast<double> (stepCount);
		if (value == -1.)
			return;

		context->editPerformer->performEdit (prgChangeParamID, value);
		int32 index;
		if (auto vq = context->outputParamChanges->addParameterData (prgChangeParamID, index))
		{
			vq->addPoint (currentSampleOffset, value, index);
		}
	}

	void onOtherInput (const uint8_t* data, size_t dataSize) override
	{
		if (!context || dataSize < 2)
			return;

		constexpr auto NoteOff = 0x80;
		constexpr auto NoteOn = 0x90;
		constexpr auto Aftertouch = 0xa0;
		constexpr auto Controller = 0xb0;
		constexpr auto ProgramChange = 0xc0;
		constexpr auto ChannelPressure = 0xd0;
		constexpr auto PitchWheel = 0xe0;

		auto status = data[0] & 0xF0;
		auto channel = data[0] & 0x0F;
		switch (status)
		{
			case NoteOn:
			{
				auto pitch = data[1];
				if (data[2] != 0)
				{
					auto velocity = static_cast<float> (data[2]) / 127.;
					if (pitchToParamIDMap[channel][pitch] != 0)
						sendNoteOff (pitchToParamIDMap[channel][pitch], pitch, velocity);
					MPE::NoteID noteID;
					if (generateNewNoteID (noteID))
					{
						pitchToParamIDMap[channel][pitch] = noteID;
						sendNoteOn (noteID, pitch, velocity, channel);
					}
				}
				else
				{
					if (pitchToParamIDMap[channel][pitch] != 0)
					{
						sendNoteOff (pitchToParamIDMap[channel][pitch], pitch, 0, channel);
						pitchToParamIDMap[channel][pitch] = 0;
					}
				}
				break;
			}
			case NoteOff:
			{
				auto pitch = data[1];
				if (pitchToParamIDMap[channel][pitch] != 0)
				{
					auto velocity = static_cast<float> (data[2]) / 127.;
					sendNoteOff (pitchToParamIDMap[channel][pitch], pitch, velocity, channel);
					pitchToParamIDMap[channel][pitch] = 0;
				}
				break;
			}
			case Aftertouch:
			{
				auto value = static_cast<MPE::NormalizedValue> (data[1]) / 127.;
				sendControllerChange (ControllerNumbers::kAfterTouch, value, channel);
				break;
			}
			case Controller:
			{
				auto value = static_cast<MPE::NormalizedValue> (data[2]) / 127.;
				sendControllerChange (data[1], value, channel);
				break;
			}
			case ProgramChange:
			{
				sendProgramChange (channel, data[1]);
				break;
			}
			case ChannelPressure:
			{
				sendPolyPressure (-1, data[1], static_cast<double> (data[2]) / 127., channel);
				break;
			}
			case PitchWheel:
			{
				auto value =
				    static_cast<MPE::NormalizedValue> ((data[1] & 0x7F) + ((data[2] & 0x7F) << 7)) /
				    16383.;
				sendControllerChange (ControllerNumbers::kPitchBend, value, channel);
				break;
			}
		}
	}

	void onSysexInput (const uint8_t* data, size_t dataSize) override
	{
		// no sysex handling
	}

	// error handling
	void errorNoteDroppedBecauseNoNoteID (MPE::Pitch pitch) override {}
	void errorNoteDroppedBecauseNoteStackFull (MPE::Channel channel, MPE::Pitch pitch) override {}
	void errorNoteForNoteOffNotFound (MPE::Channel channel, MPE::Pitch pitch) override {}
	void errorProgramChangeReceivedInMPEZone () override {}
};

struct RenderProcessContext
{
	RenderProcessContext () {}
	AUHostMusicalContextBlock musicalContext;
	AUHostTransportStateBlock transportContext;
	const AudioTimeStamp* timestamp;
	ProcessContext* processContext;
	double sampleRate;

	void updateProcessContext (const RenderProcessContext& renderContext)
	{
		Float64 currentTempo = 0;
		double timeSignatureNumerator = 0;
		NSInteger timeSignatureDenominator = 0;
		Float64 currentBeatPosition = 0;
		NSInteger sampleOffsetToNextBeat = 0;
		Float64 currentMeasureDownbeatPosition = 0;
		AUHostTransportStateFlags transportStateFlags;
		double currentSamplePosition = 0;
		double cycleStartBeatPosition = 0;
		double cycleEndBeatPosition = 0;

		if (musicalContext)
		{
			musicalContext (&currentTempo, &timeSignatureNumerator, &timeSignatureDenominator,
			                &currentBeatPosition, &sampleOffsetToNextBeat,
			                &currentMeasureDownbeatPosition);

			processContext->state |= ProcessContext::kTempoValid | ProcessContext::kTimeSigValid |
			                         ProcessContext::kProjectTimeMusicValid |
			                         ProcessContext::kBarPositionValid |
			                         ProcessContext::kClockValid;
			processContext->tempo = currentTempo;
			processContext->projectTimeMusic = currentBeatPosition;
			processContext->timeSigNumerator = timeSignatureNumerator;
			processContext->timeSigDenominator = (int32)timeSignatureDenominator;
			processContext->samplesToNextClock = (int32)sampleOffsetToNextBeat;
			processContext->barPositionMusic = currentMeasureDownbeatPosition;
		}

		if (transportContext)
		{
			transportContext (&transportStateFlags, &currentSamplePosition, &cycleStartBeatPosition,
			                  &cycleEndBeatPosition);

			processContext->state |= ProcessContext::kCycleValid;
			processContext->cycleStartMusic = cycleStartBeatPosition;
			processContext->cycleEndMusic = cycleEndBeatPosition;
			processContext->projectTimeSamples = currentSamplePosition;

			switch (transportStateFlags)
			{
				case AUHostTransportStateMoving:
					processContext->state |= ProcessContext::kPlaying;
					break;
				case AUHostTransportStateRecording:
					processContext->state |= ProcessContext::kRecording;
					break;
				case AUHostTransportStateCycling:
					processContext->state |= ProcessContext::kCycleActive;
					break;
			}
		}

		processContext->sampleRate = sampleRate;
		processContext->systemTime = timestamp->mHostTime;
	}
};

} // Vst
} // Steinberg

//------------------------------------------------------------------------
// AUv3Wrapper main class
//------------------------------------------------------------------------
#pragma mark - AUv3Wrapper main class (implementing AUAudioUnit)

using namespace Steinberg;
using namespace Vst;
@interface AUv3Wrapper ()

@property AUAudioUnitBusArray* inputBusArray;
@property AUAudioUnitBusArray* outputBusArray;
@property (nonatomic, readonly) AUParameterTree* parameterTree;
@property IEditController* editcontroller;

@end

@implementation AUv3Wrapper
{
	std::vector<BufferedInputBus> inputBusBuffers;
	std::vector<BufferedOutputBus> outputBusBuffers;

	AUHostMusicalContextBlock musicalContext;
	AUHostTransportStateBlock transportContext;

	AUParameterObserverToken parameterObserverToken;

	NSDictionary<NSString*, id>* fullStateVar;
	NSMutableArray<AUAudioUnitPreset*>* factoryPresetsVar;
	NSArray<NSNumber*>* channelCapabilitiesArray;
	AUParameterTree* parameterTreeVar;

	IPtr<IAudioProcessor> audioProcessor;
	IMidiMapping* midiMapping;

	HostProcessData processData;
	ParameterChanges inputParamChanges;
	ParameterChanges outputParamChanges;
	ParameterChangeTransfer transferParamChanges;
	ParameterChangeTransfer outputParamTransfer;
	ProcessContext processContext;
	EventList inputEvents;
	EventList outputEvents;
	AUHostApplication hostAppContext;
	MPEHandler mpeHandler;
	MPEHandler::Context mpeHandlerContext;
	std::unique_ptr<MPE::Processor> mpeProcessor;

	UnitInfoMap unitInfos;
	ParameterGroupVector parameterGroups;

	NoteInstanceID noteCounter;
	double sampleRate;
	int32 bypassParamID;

	int32 numPresets;
	int32 factoryProgramChangedID;

	bool isInstrument;
	bool isBypassed;
	bool canProcessInPlaceValue;
	bool renderingOfflineValue;

	// Midi Stuff
	int32 midiOutCount; // currently only 0 or 1 supported

	NSMutableArray<NSNumber*>* overviewParams;

	std::unique_ptr<TimerHelper> timerHelper;
	std::unique_ptr<ComponentHelper> componentHelper;

	AUAudioUnitPreset* currentPreset;
	NSInteger currentFactoryPresetIndex;
	AUParameterObserverToken bypassObserverToken;
}

//------------------------------------------------------------------------
// MARK: AUAudioUnit init & dealloc
//------------------------------------------------------------------------
- (instancetype)initWithComponentDescription:(AudioComponentDescription)componentDescription
                                     options:(AudioComponentInstantiationOptions)options
                                       error:(NSError**)outError
{
	self = [super initWithComponentDescription:componentDescription options:options error:outError];

	if (self == nil)
		return nil;

	// set instance variables
	sampleRate = 44100.0;
	bypassParamID = -1;
	factoryProgramChangedID = -1;
	isInstrument = false;
	isBypassed = false;
	outputParamTransfer.setMaxParameters (500);
	transferParamChanges.setMaxParameters (500);

	componentHelper = std::unique_ptr<ComponentHelper> (new ComponentHelper ((__bridge void*)self));
	timerHelper = std::unique_ptr<TimerHelper> (new TimerHelper ((__bridge void*)self));

	canProcessInPlaceValue = false;
	renderingOfflineValue = false;

	processData.processContext = &processContext;
	processData.inputParameterChanges = &inputParamChanges;
	processData.outputParameterChanges = &outputParamChanges;
	inputEvents.setMaxSize (128);
	outputEvents.setMaxSize (128);
	processData.inputEvents = &inputEvents;
	processData.outputEvents = &outputEvents;
	hostAppContext.wrapper = self;

	// create the channel capabilities
	NSBundle* mainBundle = [NSBundle mainBundle];
	NSString* test = [mainBundle objectForInfoDictionaryKey:@"SupportedNumChannels"];
	NSMutableArray* supportedNumChannelsArray = [NSMutableArray array];

	for (int i = 0; i < test.length; i++)
	{
		NSString* newString = [test substringWithRange:NSMakeRange (i, 1)];
		[supportedNumChannelsArray addObject:[NSNumber numberWithInt:[newString intValue]]];
	}

	channelCapabilitiesArray = supportedNumChannelsArray;

	// load VST processor and controller
	[self loadVst];

	// initialize busses
	[self initializeBusses];

	// initialize parameters
	[self initializeParameters];

	// initialize presets
	[self loadPresetList];

	// MPE Support
	mpeProcessor = std::unique_ptr<MPE::Processor> (new MPE::Processor (&mpeHandler));
	mpeHandlerContext.mpeProcessor = mpeProcessor.get ();
	mpeHandlerContext.midiMapping = midiMapping;
	mpeHandlerContext.inputEvents = &inputEvents;
	mpeHandlerContext.outputParamChanges = &outputParamChanges;
	mpeHandlerContext.editPerformer = componentHelper.get ();

	[self onNoteExpressionChanged];

	return self;
}

//------------------------------------------------------------------------
- (void)onNoteExpressionChanged
{
	if (!self.supportsMPE)
		return;

	FUnknownPtr<INoteExpressionPhysicalUIMapping> puiMapping (_editcontroller);
	if (puiMapping)
	{
		PhysicalUIMap map[3];
		map[0].physicalUITypeID = kPUIXMovement;
		map[1].physicalUITypeID = kPUIYMovement;
		map[2].physicalUITypeID = kPUIPressure;
		PhysicalUIMapList mapList {3, map};

		// We can only map one bus and one channel with MPE in AUv3
		if (puiMapping->getPhysicalUIMapping (0, 0, mapList) == kResultTrue)
		{
			mpeHandlerContext.mpeMap[0] = map[2].noteExpressionTypeID;
			mpeHandlerContext.mpeMap[1] = map[0].noteExpressionTypeID;
			mpeHandlerContext.mpeMap[2] = map[1].noteExpressionTypeID;
		}
	}
}

//------------------------------------------------------------------------
- (BOOL)enableMPESupport:(BOOL)state
{
	if (!mpeProcessor)
		return NO;

	mpeHandlerContext.enableMPEProcessing.store (state);
	return YES;
}

//------------------------------------------------------------------------
- (BOOL)setMPEInputDeviceMasterChannel:(NSInteger)masterChannel
                    memberBeginChannel:(NSInteger)memberBeginChannel
                      memberEndChannel:(NSInteger)memberEndChannel
{
	if (!mpeProcessor)
		return NO;
	auto setup = mpeProcessor->getSetup ();
	setup.masterChannel = static_cast<MPE::Channel> (masterChannel);
	setup.memberChannelBegin = static_cast<MPE::Channel> (memberBeginChannel);
	setup.memberChannelEnd = static_cast<MPE::Channel> (memberEndChannel);
	mpeProcessor->changeSetup (setup);
	return YES;
}

//------------------------------------------------------------------------
- (void)dealloc
{
	if (audioProcessor)
	{
		FUnknownPtr<IEditController> combined (audioProcessor);
		if (!combined)
		{
			FUnknownPtr<IConnectionPoint> controllerConnectionPoint (_editcontroller);
			FUnknownPtr<IConnectionPoint> processorConnectionPoint (audioProcessor);
			if (controllerConnectionPoint && processorConnectionPoint)
			{
				controllerConnectionPoint->disconnect (processorConnectionPoint);
				processorConnectionPoint->disconnect (controllerConnectionPoint);
			}
		}
	}

	if (midiMapping)
	{
		midiMapping->release ();
		midiMapping = 0;
	}

	if (_editcontroller)
	{
		_editcontroller->setComponentHandler (0);
		uint32 refCount = _editcontroller->addRef ();
		if (refCount == 2)
			_editcontroller->terminate ();

		_editcontroller->release ();
		_editcontroller->release ();
		_editcontroller = nil;
	}

	if (audioProcessor)
	{
		FUnknownPtr<IPluginBase> (audioProcessor)->terminate ();
		audioProcessor = 0;
	}

	if (parameterObserverToken != nullptr)
	{
		[parameterTreeVar removeParameterObserver:parameterObserverToken];
		parameterObserverToken = nullptr;
	}

	timerHelper = 0;
	componentHelper = 0;
}

//------------------------------------------------------------------------
// MARK: Helper Functions
//------------------------------------------------------------------------
- (NSString*)getParameterStringFromValue:(int)tag value:(AUValue)value
{
	String128 paramString;

	if (_editcontroller->getParamStringByValue (tag, value, paramString) == kResultTrue)
	{
		UString128 str (paramString);
		return [NSString stringWithCharacters:(const UniChar*)paramString length:str.getLength ()];
	}

	return @"";
}

//------------------------------------------------------------------------
- (AUValue)getParameterValueFromString:(int)tag string:(NSString*)nsString
{
	ParamValue outValue;
	CFStringRef cfString = (__bridge CFStringRef)nsString;

	String128 string128;
	CFStringGetCharacters (cfString,
	                       CFRangeMake (0, std::max<CFIndex> (128, CFStringGetLength (cfString))),
	                       (UniChar*)string128);

	_editcontroller->getParamValueByString (tag, string128, outValue);

	return (float)outValue;
}

//------------------------------------------------------------------------
- (void)loadVst
{
	FUnknownPtr<IPluginFactory2> factory (owned (GetPluginFactory ()));
	if (factory)
	{
		// find first audio processor class
		for (int32 i = 0; i < factory->countClasses (); i++)
		{
			PClassInfo2 ci;
			if (factory->getClassInfo2 (i, &ci) == kResultTrue)
			{
				if (strcmp (ci.category, kVstAudioEffectClass) == 0)
				{
					IAudioProcessor* proc = 0;
					if (factory->createInstance (ci.cid, IAudioProcessor::iid, (void**)&proc) ==
					    kResultTrue)
					{
						audioProcessor = owned (proc);
						ConstString plugCategory (ci.subCategories);
						if (plugCategory.findFirst ("Instrument", -1,
						                            ConstString::kCaseInsensitive) >= 0)
							isInstrument = true;
						break;
					}
				}
			}
		}
	}

	if (audioProcessor)
	{
		if (FUnknownPtr<IPluginBase> (audioProcessor)->initialize (hostAppContext.unknownCast ()) !=
		    kResultTrue)
			return;

		if (audioProcessor->queryInterface (IEditController::iid, (void**)&_editcontroller) !=
		    kResultTrue)
		{
			FUnknownPtr<IComponent> component (audioProcessor);
			if (component)
			{
				TUID ccid;
				if (component->getControllerClassId (ccid) == kResultTrue)
				{
					if (factory->createInstance (ccid, IEditController::iid,
					                             (void**)&_editcontroller) == kResultTrue)
					{
						if (_editcontroller->initialize (hostAppContext.unknownCast ()) !=
						    kResultTrue)
						{
							_editcontroller->release ();
							_editcontroller = 0;
							return;
						}

						FUnknownPtr<IConnectionPoint> controllerConnectionPoint (_editcontroller);
						FUnknownPtr<IConnectionPoint> processorConnectionPoint (audioProcessor);
						if (controllerConnectionPoint && processorConnectionPoint)
						{
							controllerConnectionPoint->connect (processorConnectionPoint);
							processorConnectionPoint->connect (controllerConnectionPoint);
						}

						NSMutableData* processorData = [[NSMutableData alloc] init];
						NSMutableDataIBStream stream (processorData);
						if (FUnknownPtr<IComponent> (audioProcessor)->getState (&stream) ==
						    kResultTrue)
						{
							stream.seek (0, IBStream::kIBSeekSet);
							_editcontroller->setComponentState (&stream);
						}
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------
- (void)initializeBusses
{
	NSMutableArray<AUAudioUnitBus*>* outBusArray = [[NSMutableArray<AUAudioUnitBus*> alloc] init];
	NSMutableArray<AUAudioUnitBus*>* inBusArray = [[NSMutableArray<AUAudioUnitBus*> alloc] init];

	if (_editcontroller)
	{
		_editcontroller->setComponentHandler (componentHelper.get ());

		FUnknownPtr<IComponent> component (audioProcessor);
		int32 inputBusCount = component->getBusCount (kAudio, kInput);
		int32 outputBusCount = component->getBusCount (kAudio, kOutput);

		// create input busses
		[self createBusses:true busCount:inputBusCount busArray:inBusArray component:component];

		// create output busses
		[self createBusses:false busCount:outputBusCount busArray:outBusArray component:component];

		processData.prepare (*component, 0, kSample32);

		midiOutCount = component->getBusCount (kEvent, kOutput);

		_editcontroller->queryInterface (IMidiMapping::iid, (void**)&midiMapping);
	}

	// create the input and output bus arrays (AUAudioUnitBusArray)
	_inputBusArray = [[AUAudioUnitBusArray alloc] initWithAudioUnit:self
	                                                        busType:AUAudioUnitBusTypeInput
	                                                         busses:inBusArray];

	// create the input and output bus arrays.
	_outputBusArray = [[AUAudioUnitBusArray alloc] initWithAudioUnit:self
	                                                         busType:AUAudioUnitBusTypeOutput
	                                                          busses:outBusArray];
}

//------------------------------------------------------------------------
- (void)createBusses:(bool)isInput
            busCount:(int)busCount
            busArray:(NSMutableArray<AUAudioUnitBus*>*)busArray
           component:(FUnknownPtr<IComponent>)component
{
	SpeakerArrangement sa;
	int inputOrOutput = isInput ? kInput : kOutput;

	for (int32 busNum = 0; busNum < busCount; busNum++)
	{
		AUAudioUnitBus* bus;
		AVAudioFormat* customFormat;

		if (audioProcessor->getBusArrangement (inputOrOutput, busNum, sa) == kResultTrue)
		{
			customFormat = [[AVAudioFormat alloc]
			    initStandardFormatWithSampleRate:sampleRate
			                            channels:SpeakerArr::getChannelCount (sa)];

			bus = [[AUAudioUnitBus alloc] initWithFormat:customFormat error:nil];
			bus.maximumChannelCount = customFormat.channelCount;
		}

		BusInfo info = {0};
		if (component->getBusInfo (kAudio, inputOrOutput, busNum, info) == kResultTrue)
		{
			UString128 str (info.name);
			NSString* convertedNSString =
			    [NSString stringWithCharacters:(const UniChar*)info.name length:str.getLength ()];
			bus.name = convertedNSString;
		}

		component->activateBus (kAudio, kInput, busNum, false);

		if (bus != nil)
			[busArray addObject:bus];
	}
}

//------------------------------------------------------------------------
- (void)allocateBusBuffers
{
	FUnknownPtr<IComponent> component (audioProcessor);

	for (int i = 0; i < component->getBusCount (kAudio, kInput); i++)
	{
		BufferedInputBus bus;
		bus.init (_inputBusArray[i].format, _inputBusArray[i].format.channelCount);
		bus.allocateRenderResources (self.maximumFramesToRender);
		inputBusBuffers.push_back (bus);
	}

	for (int i = 0; i < component->getBusCount (kAudio, kOutput); i++)
	{
		BufferedOutputBus bus;
		bus.init (_outputBusArray[i].format, _outputBusArray[i].format.channelCount);
		bus.allocateRenderResources (self.maximumFramesToRender);
		outputBusBuffers.push_back (bus);
	}

	// if no input bus create empty one
	if (_inputBusArray.count == 0)
	{
		AVAudioFormat* defaultFormat =
		    [[AVAudioFormat alloc] initStandardFormatWithSampleRate:sampleRate channels:2];
		BufferedInputBus bus;
		bus.init (defaultFormat, 2);
		bus.allocateRenderResources (self.maximumFramesToRender);
		inputBusBuffers.push_back (bus);
	}
}

//------------------------------------------------------------------------
- (void)initializeParameters
{
	// AUv3 Parameter Initialization
	overviewParams = [[NSMutableArray<NSNumber*> alloc] init];
	NSMutableArray* paramArray = [[NSMutableArray alloc] init];
	NSMutableArray* paramArrayWithHierarchy = [[NSMutableArray<AUParameter*> alloc] init];

	// create parameters
	[self createParameters:paramArrayWithHierarchy paramArray:paramArray];

	// create the paramArray with AUParameterGroups
	for (int32 i = 0; i < [paramArrayWithHierarchy count]; i++)
	{
		if (i == 0)
		{
			for (int32 j = 0; j < [[paramArrayWithHierarchy objectAtIndex:i] count]; j++)
				[paramArray addObject:paramArrayWithHierarchy[i][j]];
		}
		else
		{
			String str = parameterGroups.at (i - 1);
			NSString* groupName = [NSString stringWithString:[NSString stringWithUTF8String:str]];

			AUParameterGroup* newGroup =
			    [AUParameterTree createGroupWithIdentifier:groupName
			                                          name:groupName
			                                      children:paramArrayWithHierarchy[i]];

			[paramArray addObject:newGroup];
		}
	}

	// create the parameter tree
	parameterTreeVar = [AUParameterTree createTreeWithChildren:paramArray];

	// implementorValueObserver is called when a parameter changes value.
	__weak AUv3Wrapper* weakSelf = self;
	parameterTreeVar.implementorValueObserver = ^(AUParameter* param, AUValue value) {
	  [weakSelf setParameter:(int)param.address value:value];
	};

	// A function to provide string representations of parameter values.
	parameterTreeVar.implementorStringFromValueCallback =
	    ^(AUParameter* param, const AUValue* __nullable valuePtr) {
		  AUValue value = valuePtr == nil ? param.value : *valuePtr;

		  return [weakSelf getParameterStringFromValue:(int)param.address value:value];
	    };

	parameterObserverToken = [parameterTreeVar
	    tokenByAddingParameterObserver:^(AUParameterAddress address, AUValue value) {
		  dispatch_async (dispatch_get_main_queue (), ^{
			__strong AUv3Wrapper* strongSelf = weakSelf;
			[strongSelf setControllerParameter:(int)address value:value];
		  });
	    }];

	// A function to provide the parameter values of string representations.
	parameterTreeVar.implementorValueFromStringCallback = ^(AUParameter* param, NSString* string) {
	  return [weakSelf getParameterValueFromString:(int)param.address string:string];
	};
}

//------------------------------------------------------------------------
- (void)createParameters:(NSMutableArray*)paramArrayWithHierarchy
              paramArray:(NSMutableArray*)paramArray
{
	// for each VST3 parameter
	int32 parameterCount = _editcontroller->getParameterCount ();
	for (int32 i = 0; i < parameterCount; i++)
	{
		ParameterInfo pi = {0};
		_editcontroller->getParameterInfo (i, pi);

		if (pi.flags & ParameterInfo::kIsBypass)
		{
			bypassParamID = pi.id;
		}
		else
		{
			// Build parameter group if possible, maps to the AUParameterGroup Node in the
			// parameterTree
			int32 groupIdx = [self buildParameterGroup:pi];

			// create identifier for parameter
			NSString* identifier =
			    [NSString stringWithString:[NSString stringWithFormat:@"%d", pi.id]];
			NSString* name = (NSString*)CFBridgingRelease (createCFStringFromString128 (pi.title));
			NSString* unitName =
			    (NSString*)CFBridgingRelease (createCFStringFromString128 (pi.units));
			AUParameterAddress address = static_cast<AUParameterAddress> (pi.id);

			// create flags for parameter
			AudioUnitParameterOptions flagsToSet =
			    kAudioUnitParameterFlag_ValuesHaveStrings | kAudioUnitParameterFlag_IsReadable;

			if (!(pi.flags & ParameterInfo::kIsReadOnly))
				flagsToSet |= kAudioUnitParameterFlag_IsWritable;
			if (!(pi.flags & ParameterInfo::kCanAutomate))
				flagsToSet |= kAudioUnitParameterFlag_NonRealTime;
			if (pi.stepCount == 1)
				flagsToSet = kAudioUnitParameterUnit_Boolean;

			// create the parameter
			AUParameter* sPar =
			    [AUParameterTree createParameterWithIdentifier:identifier
			                                              name:name
			                                           address:address
			                                               min:0.0
			                                               max:1.0
			                                              unit:kAudioUnitParameterUnit_CustomUnit
			                                          unitName:unitName
			                                             flags:flagsToSet
			                                      valueStrings:nil
			                               dependentParameters:nil];

			// initialize the parameter values
			sPar.value = pi.defaultNormalizedValue;
			[self setParameter:pi.id value:pi.defaultNormalizedValue];
			[self setControllerParameter:pi.id value:pi.defaultNormalizedValue];

			// add parameter to the paramArrayWithHierarchy (with hierarchy/AUParameterGroups)
			// initialize paramArrayWithHierarchy once (if i == 0)
			if (i == 0)
			{
				bool rootInitialized = false;
				for (auto it = unitInfos.begin (); it != unitInfos.end (); ++it)
				{
					[paramArrayWithHierarchy addObject:[[NSMutableArray alloc] init]];

					if (it->second.parentUnitId == -1)
						rootInitialized = true;
				}

				if (!rootInitialized)
					[paramArrayWithHierarchy addObject:[[NSMutableArray alloc] init]];
			}

			NSUInteger groupOfParam = (NSUInteger)groupIdx;
			[[paramArrayWithHierarchy objectAtIndex:groupOfParam] addObject:sPar];
			[overviewParams addObject:[NSNumber numberWithUnsignedLongLong:address]];
		}
	}
}

//------------------------------------------------------------------------
- (int32)buildParameterGroup:(ParameterInfo)pi
{
	int32 groupIdx = 0; // Zero means parameter is on top level node of parameter tree

	FUnknownPtr<IUnitInfo> unitInfoController (_editcontroller);
	if (unitInfoController)
	{
		if (unitInfos.empty ())
			buildUnitInfos (unitInfoController, unitInfos);

		String fullParamName;
		ConstString separator (STR ("."));
		bool insertSeparator = false;

		UnitInfoMap::const_iterator it = unitInfos.find (pi.unitId);
		while (it != unitInfos.end () && it->second.id != kRootUnitId)
		{
			ConstString unitName (it->second.name);
			if (unitName.length () > 0)
			{
				if (insertSeparator)
					fullParamName.insertAt (0, separator);
				insertSeparator = true;
				fullParamName.insertAt (0, unitName);
			}
			it = unitInfos.find (it->second.parentUnitId);
		}
		if (!fullParamName.isEmpty ())
		{
			bool insertGroup = true;
			groupIdx = 1;
			for (int32 i = 0; i < parameterGroups.size (); i++)
			{
				const String& str = parameterGroups.at (i);
				if (str.compare (fullParamName) == 0)
					break;

				++groupIdx;
			}

			if (insertGroup)
				parameterGroups.push_back (fullParamName);
		}
	}
	return groupIdx;
}

//------------------------------------------------------------------------
- (void)setupProcessing
{
	if (audioProcessor && _editcontroller)
	{
		FUnknownPtr<IComponent> component (audioProcessor);

		// after set Bus Arrangement, the channelbuffers may need to be reallocated -> hence the
		// second prepare!
		processData.prepare (*component, 0, kSample32);

		ProcessSetup ps;
		ps.sampleRate = sampleRate;
		ps.maxSamplesPerBlock = self.maximumFramesToRender;
		ps.symbolicSampleSize = kSample32;
		ps.processMode = kRealtime;

		audioProcessor->setupProcessing (ps);

		component->setActive (true);

		audioProcessor->setProcessing (true);
	}
}

//------------------------------------------------------------------------
- (bool)getProgramListAndUnit:(int32)midiChannel
                       unitId:(UnitID&)unitId
                programListId:(ProgramListID&)programListId
{
	programListId = kNoProgramListId;
	unitId = -1;

	IUnitInfo* unitInfo = NULL;

	if (_editcontroller &&
	    _editcontroller->queryInterface (IUnitInfo::iid, (void**)&unitInfo) == kResultTrue &&
	    unitInfo)
	{
		if (unitInfo->getUnitByBus (kEvent, kInput, 0, midiChannel, unitId) == kResultTrue)
		{
			int32 unitCount = unitInfo->getUnitCount ();
			for (int32 i = 0; i < unitCount; i++)
			{
				UnitInfo unitInfoStruct = {0};
				if (unitInfo->getUnitInfo (i, unitInfoStruct) == kResultTrue)
				{
					if (unitId == unitInfoStruct.id)
					{
						programListId = unitInfoStruct.programListId;
						unitInfo->release ();
						return programListId != kNoProgramListId;
					}
				}
			}
		}

		unitInfo->release ();
	}
	return false;
}

//------------------------------------------------------------------------
- (void)loadPresetList
{
	std::vector<ParameterInfo> programParameters;
	bool noPresets = true;

	// for each VST3 parameter
	int32 parameterCount = _editcontroller->getParameterCount ();
	for (int32 i = 0; i < parameterCount; i++)
	{
		ParameterInfo pi = {0};
		_editcontroller->getParameterInfo (i, pi);

		// do not register bypass
		if ((pi.flags & ParameterInfo::kIsBypass) != 0)
		{
			continue;
		}
		else if ((pi.flags & ParameterInfo::kIsProgramChange) != 0)
		{
			// see if parameter is programChange
			noPresets = false;
			programParameters.push_back (pi);
		}
	}

	if (noPresets)
		return;

	// assign programChanges
	for (int32 midiChannel = 0; midiChannel < 16; ++midiChannel)
	{
		mpeHandlerContext.programChangeParameters[midiChannel] = kNoParamId;
		UnitID unitId;
		ProgramListID programListId;
		if (![self getProgramListAndUnit:midiChannel unitId:unitId programListId:programListId])
			continue;
		for (int32 i = 0; i < (int32)programParameters.size (); i++)
		{
			const ParameterInfo& paramInfo = programParameters.at (i);
			if (paramInfo.unitId == unitId)
			{
				mpeHandlerContext.programChangeParameters[midiChannel] = paramInfo.id;
				mpeHandlerContext.programChangeParameterStepCount[midiChannel] =
				    paramInfo.stepCount;
				break;
			}
		}
	}

	FUnknownPtr<IUnitInfo> unitInfoController (_editcontroller);

	factoryPresetsVar = [[NSMutableArray<AUAudioUnitPreset*> alloc] init];

	// let's see if there's a preset list (take the first one)
	if (!unitInfoController)
		return;

	ProgramListInfo programListInfo;
	if (unitInfoController->getProgramListInfo (0, programListInfo) != kResultTrue)
		return;

	factoryProgramChangedID = -1;
	numPresets = programListInfo.programCount;
	if (programListInfo.programCount > 0)
	{
		UnitID unitId = -1;

		// find the unit supporting this ProgramList
		IUnitInfo* unitInfo = NULL;
		if (_editcontroller->queryInterface (IUnitInfo::iid, (void**)&unitInfo) == kResultTrue &&
		    unitInfo)
		{
			int32 unitCount = unitInfo->getUnitCount ();
			for (int32 i = 0; i < unitCount; i++)
			{
				UnitInfo unitInfoStruct = {0};
				if (unitInfo->getUnitInfo (i, unitInfoStruct) == kResultTrue)
				{
					if (programListInfo.id == unitInfoStruct.programListId)
					{
						unitId = unitInfoStruct.id;
						break;
					}
				}
			}

			unitInfo->release ();
		}

		if (unitId != -1)
		{
			// find the associated ProgramChange parameter ID
			for (int32 i = 0; i < (int32)programParameters.size (); i++)
			{
				const ParameterInfo& paramInfo = programParameters.at (i);
				if (paramInfo.unitId == unitId)
				{
					factoryProgramChangedID = paramInfo.id;
					break;
				}
			}

			if (factoryProgramChangedID != -1)
			{
				for (int32 i = 0; i < programListInfo.programCount; i++)
				{
					String128 name;
					unitInfoController->getProgramName (programListInfo.id, i, name);

					UString128 str (name);
					NSString* convertedNSString =
					    [NSString stringWithCharacters:(const UniChar*)name
					                            length:str.getLength ()];

					AUAudioUnitPreset* preset = [[AUAudioUnitPreset alloc] init];
					[preset setName:convertedNSString];
					[preset setNumber:static_cast<NSInteger> (i)];

					[factoryPresetsVar addObject:preset];
				}
			}
		}
	}
}

//------------------------------------------------------------------------
- (void)syncParameterValues
{
	if (!_editcontroller)
		return;

	int32 parameterCount = _editcontroller->getParameterCount ();

	for (int32 i = 0; i < parameterCount; i++)
	{
		ParameterInfo pi;
		_editcontroller->getParameterInfo (i, pi);

		if (pi.flags & ParameterInfo::kIsBypass)
		{
			bypassParamID = pi.id;
		}
		else
		{
			ParamValue value = _editcontroller->getParamNormalized (pi.id);
			// set the AU parameter value
			AUParameter* parameterToChange = [parameterTreeVar parameterWithAddress:pi.id];
			parameterToChange.value = value;
		}
	}
}

//------------------------------------------------------------------------
- (void)setParameter:(int)tag value:(double)value
{
	if (tag == factoryProgramChangedID)
	{
		// load factory preset here
		float normalizedValue = (float)value / (float)numPresets;
		transferParamChanges.addChange (factoryProgramChangedID, normalizedValue, 0);
	}

	transferParamChanges.addChange (tag, value, 0);
}

//------------------------------------------------------------------------
- (void)setControllerParameter:(int)tag value:(double)value
{
	if (_editcontroller && tag != factoryProgramChangedID)
		_editcontroller->setParamNormalized (tag, value);
}

//------------------------------------------------------------------------
- (void)performEdit:(int)tag value:(double)value
{
	AUParameter* parameterToChange = [parameterTreeVar parameterWithAddress:tag];

	if (parameterObserverToken != nullptr)
		[parameterToChange setValue:value originator:parameterObserverToken];
	else
		[parameterToChange setValue:value];
}

//------------------------------------------------------------------------
- (void)onTimer
{
	ParamID pid;
	ParamValue value;
	int32 sampleOffset;

	while (outputParamTransfer.getNextChange (pid, value, sampleOffset))
	{
		_editcontroller->setParamNormalized (pid, value);
		AUParameter* parameterToChange = [parameterTreeVar parameterWithAddress:pid];
		parameterToChange.value = value;
	}

	if (!isBypassed)
		return;

	ProcessData bypassData = processData;
	bypassData.numSamples = 0;
	inputParamChanges.clearQueue ();
	transferParamChanges.transferChangesTo (inputParamChanges);

	if (inputParamChanges.getParameterCount () > 0)
	{
		audioProcessor->process (bypassData);
		outputParamTransfer.transferChangesFrom (outputParamChanges);
		outputParamChanges.clearQueue ();
	}
}

//------------------------------------------------------------------------
- (BOOL)validateChannelConfig
{
	FUnknownPtr<IComponent> component (audioProcessor);
	int32 inputBusCount = component->getBusCount (kAudio, kInput);
	int32 outputBusCount = component->getBusCount (kAudio, kOutput);
	SpeakerArrangement inputs[inputBusCount];
	SpeakerArrangement outputs[outputBusCount];
	int32 maxBusCount = std::max (inputBusCount, outputBusCount);
	BOOL didChannelConfigPass = YES;

	for (int32 i = 0; i < maxBusCount; i++)
	{
		int32 inChannelCount = 0;
		int32 outChannelCount = 0;
		if (inputBusCount > i)
			inChannelCount = _outputBusArray[i].format.channelCount;
		if (outputBusCount > i)
			outChannelCount = _outputBusArray[i].format.channelCount;

		didChannelConfigPass = [self validateChannelPair:inChannelCount
		                                   inChannelsOut:outChannelCount
		                                            info:channelCapabilitiesArray
		                                     numChanInfo:([channelCapabilitiesArray count] / 2)];
	}

	for (int32 element = 0; element < inputBusCount; element++)
		inputs[element] =
		    numChannelsToSpeakerArrangement (_inputBusArray[element].format.channelCount);

	for (int32 element = 0; element < outputBusCount; element++)
		outputs[element] =
		    numChannelsToSpeakerArrangement (_outputBusArray[element].format.channelCount);

	if (audioProcessor->setBusArrangements (inputs, inputBusCount, outputs, outputBusCount) !=
	    kResultTrue)
		didChannelConfigPass = NO;

	return didChannelConfigPass;
}

//------------------------------------------------------------------------
- (BOOL)validateChannelPair:(int)inChannelsIn
              inChannelsOut:(int)inChannelsOut
                       info:(NSArray<NSNumber*>*)info
                numChanInfo:(long)numChanInfo
{
	// we've the following cases (some combinations) to test here:
	/*
	 >0		An explicit number of channels on either side
	 0		that side (generally input!) has no elements
	 -1		wild card:
	 -1,-1	any num channels as long as same channels on in and out
	 -1,-2	any num channels channels on in and out - special meaning
	 -2+ 	indicates total num channs AU can handle
	 - elements configurable to any num channels,
	 - element count in scope must be writable
	 */

	// now chan layout can contain -1 for either scope (ie. doesn't care)
	for (unsigned int i = 0; i < numChanInfo; ++i)
	{
		int inputValue = (int)[info[(i * 2)] integerValue];
		int outputValue = (int)[info[(i * 2) + 1] integerValue];

		// less than zero on both sides - check for special attributes
		if ((inputValue < 0) && (outputValue < 0))
		{
			// these are our wild card matches
			if (inputValue == -1 && outputValue == -1)
			{
				if (inChannelsIn && inChannelsOut)
				{
					if (inChannelsOut == inChannelsIn)
						return true;
				}
				else
					return true; // if one of these is zero, then a -1 means any
			}
			else if ((inputValue == -1 && outputValue == -2) ||
			         (inputValue == -2 && outputValue == -1))
			{
				return true;
			}
			// these are our total num channels matches
			// element count MUST be writable
			else
			{
				bool outWrite = false;
				bool inWrite = false;
				// IsElementCountWritable (kAudioUnitScope_Output, outWrite);
				// IsElementCountWritable (kAudioUnitScope_Input, inWrite);
				if (inWrite && outWrite)
				{
					if ((inChannelsOut <= abs (outputValue)) && (inChannelsIn <= abs (inputValue)))
						return true;
				}
			}
		}

		// special meaning on input, specific num on output
		else if (inputValue < 0)
		{
			if (outputValue == inChannelsOut)
			{
				// can do any in channels
				if (inputValue == -1)
				{
					return true;
				}
				// total chans on input
				else
				{
					bool inWrite = false;
					// IsElementCountWritable (kAudioUnitScope_Input, inWrite);
					if (inWrite && (inChannelsIn <= abs (inputValue)))
						return true;
				}
			}
		}

		// special meaning on output, specific num on input
		else if (outputValue < 0)
		{
			if (inputValue == inChannelsIn)
			{
				// can do any out channels
				if (outputValue == -1)
				{
					return true;
				}
				// total chans on output
				else
				{
					bool outWrite = false;
					// IsElementCountWritable (kAudioUnitScope_Output, outWrite);
					if (outWrite && (inChannelsOut <= abs (outputValue)))
					{
						return true;
					}
				}
			}
		}

		// both chans in struct >= 0 - thus has to explicitly match
		else if ((inputValue == inChannelsIn) && (outputValue == inChannelsOut))
		{
			return true;
		}

		// now check to see if a wild card on the args (inChannelsIn or inChannelsOut chans is zero)
		// is found
		// tells us to match just one side of the scopes
		else if (inChannelsIn == 0)
		{
			if (outputValue == inChannelsOut)
				return true;
		}
		else if (inChannelsOut == 0)
		{
			if (inputValue == inChannelsIn)
				return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------
// MARK: AUAudioUnit Overrides
//------------------------------------------------------------------------
- (AUParameterTree*)parameterTree
{
	return parameterTreeVar;
}

//------------------------------------------------------------------------
- (NSArray<AUAudioUnitPreset*>*)factoryPresets
{
	return factoryPresetsVar;
}

//------------------------------------------------------------------------
- (NSArray<NSNumber*>*)channelCapabilities
{
	return channelCapabilitiesArray;
}

//------------------------------------------------------------------------
- (AUAudioUnitBusArray*)inputBusses
{
	return _inputBusArray;
}

//------------------------------------------------------------------------
- (AUAudioUnitBusArray*)outputBusses
{
	return _outputBusArray;
}

//------------------------------------------------------------------------
- (BOOL)canProcessInPlace
{
	return canProcessInPlaceValue;
}

//------------------------------------------------------------------------
- (BOOL)isRenderingOffline
{
	return renderingOfflineValue;
}

//------------------------------------------------------------------------
- (BOOL)supportsMPE
{
	if (_editcontroller)
	{
		FUnknownPtr<INoteExpressionController> nec (_editcontroller);
		if (nec)
			return YES;
	}
	return NO;
}

//------------------------------------------------------------------------
- (void)setRenderingOffline:(BOOL)renderingOffline
{
	// set offline rendering of vst3 to true
}

//------------------------------------------------------------------------
- (NSTimeInterval)latency
{
	if (audioProcessor)
		return (audioProcessor->getLatencySamples () / sampleRate);
	return 0.0;
}

//------------------------------------------------------------------------
- (NSTimeInterval)tailTime
{
	if (audioProcessor)
		return (audioProcessor->getTailSamples () / sampleRate);
	return 0.0;
}

//------------------------------------------------------------------------
- (BOOL)shouldChangeToFormat:(AVAudioFormat*)format forBus:(AUAudioUnitBus*)bus
{
	// implement ChangeStreamFormat from AUv2 Wrapper here
	return [super shouldChangeToFormat:format forBus:bus];
}

//------------------------------------------------------------------------
- (NSArray<NSNumber*>*)parametersForOverviewWithCount:(NSInteger)count
{
	NSInteger n = [overviewParams count];

	if (count >= n)
		return overviewParams;

	NSMutableArray<NSNumber*>* overviewParamsWithRange =
	    [[NSMutableArray<NSNumber*> alloc] initWithArray:overviewParams];
	NSRange range = NSMakeRange (count, (n - count));
	[overviewParamsWithRange removeObjectsInRange:range];

	return overviewParamsWithRange;
}

//------------------------------------------------------------------------
- (AUAudioUnitPreset*)currentPreset
{
	if (currentPreset.number >= 0)
		// factory preset
		return [factoryPresetsVar objectAtIndex:currentFactoryPresetIndex];
	else
		// custom preset
		return currentPreset;
}

//------------------------------------------------------------------------
- (void)setCurrentPreset:(AUAudioUnitPreset*)presentPreset
{
	if (nil == presentPreset)
		return;

	// if it is a factory preset
	if (presentPreset.number >= 0)
	{
		for (AUAudioUnitPreset* factoryPreset in factoryPresetsVar)
		{
			if (presentPreset.number == factoryPreset.number)
			{

				if (numPresets > 0)
				{
					float normalizedValue = (float)presentPreset.number / (float)numPresets;
					transferParamChanges.addChange (factoryProgramChangedID, normalizedValue, 0);
					_editcontroller->setParamNormalized (factoryProgramChangedID, normalizedValue);
				}

				// set factory preset as current
				currentPreset = presentPreset;
				currentFactoryPresetIndex = factoryPreset.number;
				NSLog (@"currentPreset Factory: %ld, %@\n", (long)currentFactoryPresetIndex,
				       factoryPreset.name);

				break;
			}
		}
	}
	else if (nil != presentPreset.name)
		// set custom preset as current
		currentPreset = presentPreset;
}

//------------------------------------------------------------------------
- (NSDictionary<NSString*, id>*)fullState
{
	// should flush parameters of VST to save correct state (when not in playback mode)

	NSMutableDictionary<NSString*, id>* dict = [[NSMutableDictionary<NSString*, id> alloc] init];

	NSDictionary<NSString*, id>* superDict = [super fullState];

	if (superDict != nullptr)
		[dict addEntriesFromDictionary:superDict];

	NSMutableData* processorData = [[NSMutableData alloc] init];
	NSMutableData* controllerData = [[NSMutableData alloc] init];

	if (audioProcessor)
	{
		NSMutableDataIBStream stream (processorData);

		if (FUnknownPtr<IComponent> (audioProcessor)->getState (&stream) != kResultTrue)
			[processorData setLength:0];
	}

	if (_editcontroller)
	{
		NSMutableDataIBStream stream (controllerData);

		if (_editcontroller->getState (&stream) != kResultTrue)
			[controllerData setLength:0];
	}

	[dict setObject:processorData forKey:@"Processor State"];
	[dict setObject:controllerData forKey:@"Controller State"];

	return dict;
}

//------------------------------------------------------------------------
- (void)setFullState:(NSDictionary<NSString*, id>*)newFullState
{

	if (newFullState == nullptr)
		return;

	bool fromProject = false;

	fullStateVar = newFullState;

	if (!(audioProcessor && _editcontroller))
		return;

	// Note: the bypass state is not part of the AU state
	bool wasBypassed = false;
	if (bypassParamID != -1)
		wasBypassed = _editcontroller->getParamNormalized (bypassParamID) >= 0.5 ? true : false;

	NSMutableDictionary<NSString*, id>* modifiedState =
	    [[NSMutableDictionary<NSString*, id> alloc] init];
	[modifiedState addEntriesFromDictionary:newFullState];

	NSString* nsPresetKey = [[NSString alloc] initWithUTF8String:kAUPresetDataKey];
	[modifiedState removeObjectForKey:nsPresetKey];

	NSData* processorData = [modifiedState valueForKey:@"Processor State"];
	int processLen = (int)[processorData length];
	if (processLen == 0)
		return;

	if (processorData)
	{
		NSDataIBStream stream (processorData);
		if (fromProject)
			stream.getAttributes ()->setString (Vst::PresetAttributes::kStateType,
			                                    String (Vst::StateType::kProject));
		FUnknownPtr<IComponent> (audioProcessor)->setState (&stream);
	}

	NSData* controllerData = [modifiedState valueForKey:@"Controller State"];

	if (controllerData)
	{
		NSDataIBStream processorStream (processorData);
		_editcontroller->setComponentState (&processorStream);

		NSDataIBStream stream (controllerData);
		if (fromProject)
			stream.getAttributes ()->setString (Vst::PresetAttributes::kStateType,
			                                    String (Vst::StateType::kProject));
		_editcontroller->setState (&stream);

		[self syncParameterValues];
	}

	if (bypassParamID != -1)
	{
		transferParamChanges.addChange (bypassParamID, wasBypassed ? 1 : 0, 0);
		_editcontroller->setParamNormalized (bypassParamID, wasBypassed ? 1 : 0);
	}
}

//------------------------------------------------------------------------
- (BOOL)allocateRenderResourcesAndReturnError:(NSError**)outError
{
	if (![super allocateRenderResourcesAndReturnError:outError])
		return NO;

	if (![self validateChannelConfig])
	{
		if (outError)
		{
			*outError = [NSError errorWithDomain:NSOSStatusErrorDomain
			                                code:kAudioUnitErr_FormatNotSupported
			                            userInfo:nil];
		}

		// Notify superclass that initialization was not successful
		self.renderResourcesAllocated = NO;

		return NO;
	}

	[self allocateBusBuffers];

	[self setupProcessing];

	if (self.musicalContextBlock)
		musicalContext = self.musicalContextBlock;
	else
		musicalContext = nil;

	if (self.transportStateBlock)
		transportContext = self.transportStateBlock;
	else
		transportContext = nil;

	return YES;
}

//------------------------------------------------------------------------
- (void)deallocateRenderResources
{
	musicalContext = nullptr;
	transportContext = nullptr;

	for (int i = 0; i > inputBusBuffers.size (); i++)
		inputBusBuffers.at (i).deallocateRenderResources ();

	for (int i = 0; i > outputBusBuffers.size (); i++)
		outputBusBuffers.at (i).deallocateRenderResources ();

	inputBusBuffers.clear ();
	outputBusBuffers.clear ();

	if (audioProcessor)
	{
		audioProcessor->setProcessing (false);

		FUnknownPtr<IComponent> component (audioProcessor);
		component->setActive (false);
	}

	[super deallocateRenderResources];
}

//------------------------------------------------------------------------
- (AUInternalRenderBlock)internalRenderBlock
{
	return ^AUAudioUnitStatus (
	    AudioUnitRenderActionFlags* actionFlags, const AudioTimeStamp* timestamp,
	    AVAudioFrameCount frameCount, NSInteger outputBusNumber, AudioBufferList* outputData,
	    const AURenderEvent* realtimeEventListHead, AURenderPullInputBlock pullInputBlock) {

	  // process events/params
	  for (const AURenderEvent* event = realtimeEventListHead; event != nullptr;
		   event = event->head.next)
	  {
		  switch (event->head.eventType)
		  {
			  case AURenderEventMIDI:
			  case AURenderEventMIDISysEx:
			  {
				  mpeHandler.process (mpeHandlerContext, &event->MIDI, timestamp);
				  break;
			  }

			  case AURenderEventParameter:
			  case AURenderEventParameterRamp:
			  {
				  componentHelper->performEdit ((int)event->parameter.parameterAddress,
					                            event->parameter.value);
			  }
			  break;

			  default: break;
		  }
	  }

	  // prepare contexts
	  RenderProcessContext renderProcessContext = RenderProcessContext ();

	  renderProcessContext.musicalContext = musicalContext;
	  renderProcessContext.transportContext = transportContext;
	  renderProcessContext.processContext = &(processContext);
	  renderProcessContext.sampleRate = sampleRate;
	  renderProcessContext.timestamp = timestamp;

	  renderProcessContext.updateProcessContext (renderProcessContext);

	  inputParamChanges.clearQueue ();
	  transferParamChanges.transferChangesTo (inputParamChanges);
	  processData.numSamples = frameCount;

	  if (frameCount > 0)
	  {
		  FUnknownPtr<IComponent> component (audioProcessor);
		  int32 inputBusCount = component->getBusCount (kAudio, kInput);
		  int32 outputBusCount = component->getBusCount (kAudio, kOutput);
		  AudioUnitRenderActionFlags pullFlags = 0;

		  // pull input buffer
		  for (int32 i = 0; i < inputBusCount; i++)
		  {
			  UInt32 byteSize = frameCount * sizeof (float);

			  if (pullInputBlock == nullptr ||
				  inputBusBuffers.at (i).pullInput (&pullFlags, timestamp, frameCount, i,
				                                    pullInputBlock) != noErr)
			  {
				  for (int32 channel = 0; channel < inputBusBuffers.at (i).bus.format.channelCount;
					   channel++)
					  memset (
						  inputBusBuffers.at (i).mutableAudioBufferList->mBuffers[channel].mData, 0,
						  byteSize);
			  }

			  if (actionFlags != nullptr &&
				  (*actionFlags & kAudioUnitRenderAction_OutputIsSilence) != 0)
			  {
				  for (int32 channel = 0; channel < inputBusBuffers.at (i).bus.format.channelCount;
					   channel++)
					  memset (
						  inputBusBuffers.at (i).mutableAudioBufferList->mBuffers[channel].mData, 0,
						  byteSize);
			  }
		  }

		  // prepare output buffer
		  for (int32 i = 0; i < outputBusCount; i++)
			  outputBusBuffers.at (i).prepareOutputBufferList (outputData, frameCount,
				                                               i == outputBusNumber ? true : false);

		  if (outputData->mBuffers[0].mData == nullptr)
		  {
			  for (UInt32 channel = 0; channel < outputData->mNumberBuffers; channel++)
				  outputData->mBuffers[channel].mData =
					  inputBusBuffers.at (outputBusNumber)
					      .mutableAudioBufferList->mBuffers[channel]
					      .mData;
		  }

		  // copy input buffers
		  for (int32 i = 0; i < inputBusCount; i++)
		  {
			  AVAudioChannelCount channelCount = inputBusBuffers.at (i).bus.format.channelCount;
			  processData.inputs[i].numChannels = int32 (channelCount);

			  for (int32 channel = 0; channel < channelCount; channel++)
				  processData.inputs[i].channelBuffers32[channel] =
					  (Sample32*)inputBusBuffers.at (i)
					      .mutableAudioBufferList->mBuffers[channel]
					      .mData;
		  }

		  // copy output buffers
		  for (int32 i = 0; i < outputBusCount; i++)
		  {
			  AVAudioChannelCount channelCount = outputBusBuffers.at (i).bus.format.channelCount;
			  processData.outputs[i].numChannels = int32 (channelCount);

			  for (int32 channel = 0; channel < channelCount; channel++)
			  {
				  if (i == outputBusNumber)
				  {
					  processData.outputs[i].channelBuffers32[channel] =
						  (Sample32*)outputData->mBuffers[channel].mData;
				  }
				  else
				  {
					  processData.outputs[i].channelBuffers32[channel] =
						  (Sample32*)outputBusBuffers.at (i)
						      .mutableAudioBufferList->mBuffers[channel]
						      .mData;
				  }
			  }
		  }

		  // process audio
		  audioProcessor->process (processData);

		  outputParamTransfer.transferChangesFrom (outputParamChanges);
		  outputParamChanges.clearQueue ();
		  inputEvents.clear ();

		  pullFlags &= ~kAudioUnitRenderAction_OutputIsSilence;
	  }
	  return noErr;
	};
}
@end

//------------------------------------------------------------------------
//  AUv3WrapperViewController
//------------------------------------------------------------------------
#pragma mark - ViewController (implementing AUViewController and AUAudioUnitFactory)
@interface AUv3WrapperViewController ()
{
	IPlugView* plugView;
	FObject* dynlib;
	AUPlugFrame* plugFrame;
	IEditController* editController;
	BOOL isAttached;
}
@end

//------------------------------------------------------------------------
//  AUv3WrapperViewController
//------------------------------------------------------------------------
@implementation AUv3WrapperViewController

//------------------------------------------------------------------------
- (void)loadView
{
	SMTG_IOS_MAC_VIEW* view = [[SMTG_IOS_MAC_VIEW alloc] initWithFrame:CGRectMake (0, 0, 0, 0)];
	[self setView:view];
}

//------------------------------------------------------------------------
- (void)setFrame:(CGRect)newSize
{
	[super.view setFrame:newSize];
	ViewRect viewRect (0, 0, newSize.size.width, newSize.size.height);

	if (plugView)
		plugView->onSize (&viewRect);
}

//------------------------------------------------------------------------
- (AUv3Wrapper*)getAudioUnit
{
	return _audioUnit;
}

//------------------------------------------------------------------------
- (void)setAudioUnit:(AUv3Wrapper*)audioUnit
{
	if (audioUnit == nil)
		return;

	_audioUnit = audioUnit;

	editController = [_audioUnit editcontroller];
	if (editController)
		editController->addRef ();

	dispatch_async (dispatch_get_main_queue (), ^{
  //------------------------------------------------------------------------
	  //  attach from VST3Editor.mm
  //------------------------------------------------------------------------
	  FUnknownPtr<IEditController2> ec2 (editController);
	  if (ec2)
		  ec2->setKnobMode (kLinearMode);

	  // create view
	  plugView = editController->createView (ViewType::kEditor);
	  if (plugView)
	  {
		  if (plugView->isPlatformTypeSupported (SMTG_IOS_MAC_PLATFORMTYPE) == kResultTrue)
		  {
			  plugFrame = NEW AUPlugFrame (self.view);
			  plugView->setFrame (plugFrame);

			  if (plugView->attached ((__bridge void*)self.view, SMTG_IOS_MAC_PLATFORMTYPE) ==
				  kResultTrue)
			  {
				  ViewRect vr;
				  if (plugView->getSize (&vr) == kResultTrue)
				  {
					  int viewWidth = vr.right - vr.left;
					  int viewHeight = vr.bottom - vr.top;
					  CGRect newSize = CGRectMake (0, 0, viewWidth, viewHeight);
					  [self setFrame:newSize];
					  self.preferredContentSize = CGSizeMake (viewWidth, viewHeight);
				  }
			  }
		  }
		  else
		  {
			  plugView->release ();
			  plugView = 0;
		  }
	  }
	});
}

//------------------------------------------------------------------------
- (void)dealloc
{
	if (plugView)
	{
		// remove plugFrame from plugView
		if (isAttached)
		{
			plugView->setFrame (0);
			plugView->removed ();
		}

		// release plugView
		plugView->release ();

		// release plugFrame
		if (plugFrame)
			plugFrame->release ();

		// release editController
		if (editController)
		{
			editController->release ();
			editController = 0;
		}
	}

	if (dynlib)
		dynlib->release ();

	self.audioUnit = nil;
}

@end
