//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/again/source/againsimple.cpp
// Created by  : Steinberg, 04/2005
// Description : AGain Example for VST SDK 3.0
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

#include "againsimple.h"
#include "againparamids.h"
#include "againuimessagecontroller.h"
#include "version.h" // for versioning

#include "public.sdk/source/main/pluginfactory.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h" // for UString128
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/vstpresetkeys.h" // for use of IStreamAttributes

#include "vstgui/plugin-bindings/vst3editor.h"

#include <cmath>
#include <cstdio>

// this allows to enable the communication example between again and its controller
#define AGAIN_TEST 1

using namespace VSTGUI;

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// GainParameter Declaration
// example of custom parameter (overwriting to and fromString)
//------------------------------------------------------------------------
class GainParameter : public Parameter
{
public:
	GainParameter (int32 flags, int32 id);

	void toString (ParamValue normValue, String128 string) const SMTG_OVERRIDE;
	bool fromString (const TChar* string, ParamValue& normValue) const SMTG_OVERRIDE;
};

//------------------------------------------------------------------------
// GainParameter Implementation
//------------------------------------------------------------------------
GainParameter::GainParameter (int32 flags, int32 id)
{
	Steinberg::UString (info.title, USTRINGSIZE (info.title)).assign (USTRING ("Gain"));
	Steinberg::UString (info.units, USTRINGSIZE (info.units)).assign (USTRING ("dB"));

	info.flags = flags;
	info.id = id;
	info.stepCount = 0;
	info.defaultNormalizedValue = 0.5f;
	info.unitId = kRootUnitId;

	setNormalized (1.f);
}

//------------------------------------------------------------------------
void GainParameter::toString (ParamValue normValue, String128 string) const
{
	char text[32];
	if (normValue > 0.0001)
		sprintf (text, "%.2f", 20 * log10f ((float)normValue));
	else
		strcpy (text, "-oo");

	Steinberg::UString (string, 128).fromAscii (text);
}

//------------------------------------------------------------------------
bool GainParameter::fromString (const TChar* string, ParamValue& normValue) const
{
	Steinberg::UString wrapper ((TChar*)string, -1); // don't know buffer size here!
	double tmp = 0.0;
	if (wrapper.scanFloat (tmp))
	{
		// allow only values between -oo and 0dB
		if (tmp > 0.0)
			tmp = -tmp;

		normValue = expf (logf (10.f) * (float)tmp / 20.f);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
// AGain Implementation
//------------------------------------------------------------------------
AGainSimple::AGainSimple ()
: fGain (1.f)
, fGainReduction (0.f)
, fVuPPMOld (0.f)
, currentProcessMode (-1) // -1 means not initialized
, bHalfGain (false)
, bBypass (false)
{
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::initialize (FUnknown* context)
{
	tresult result = SingleComponentEffect::initialize (context);
	if (result != kResultOk)
		return result;

	//---create Audio In/Out busses------
	// we want a stereo Input and a Stereo Output
	addAudioInput  (STR16 ("Stereo In"),  SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), SpeakerArr::kStereo);

	//---create Event In/Out busses (1 bus with only 1 channel)------
	addEventInput (STR16 ("Event In"), 1);

	//---Create Parameters------------

	//---Gain parameter--
	auto* gainParam = new GainParameter (ParameterInfo::kCanAutomate, kGainId);
	parameters.addParameter (gainParam);

	//---VuMeter parameter---
	int32 stepCount = 0;
	ParamValue defaultVal = 0;
	int32 flags = ParameterInfo::kIsReadOnly;
	int32 tag = kVuPPMId;
	parameters.addParameter (USTRING ("VuPPM"), nullptr, stepCount, defaultVal, flags, tag);

	//---Bypass parameter---
	stepCount = 1;
	defaultVal = 0;
	flags = ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass;
	tag = kBypassId;
	parameters.addParameter (USTRING ("Bypass"), nullptr, stepCount, defaultVal, flags, tag);

	//---Custom state init------------

	UString str (defaultMessageText, 128);
	str.fromAscii ("Hello World!");

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::terminate ()
{
	return SingleComponentEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::setActive (TBool state)
{
#if AGAIN_TEST
	if (state)
		fprintf (stderr, "[AGainSimple] Activated \n");
	else
		fprintf (stderr, "[AGainSimple] Deactivated \n");
#endif

	// reset the VuMeter value
	fVuPPMOld = 0.f;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::process (ProcessData& data)
{
	// finally the process function
	// In this example there are 4 steps:
	// 1) Read inputs parameters coming from host (in order to adapt our model values)
	// 2) Read inputs events coming from host (we apply a gain reduction depending of the velocity of pressed key)
	// 3) Process the gain of the input buffer to the output buffer
	// 4) Write the new VUmeter value to the output Parameters queue


	//---1) Read inputs parameter changes-----------
	IParameterChanges* paramChanges = data.inputParameterChanges;
	if (paramChanges)
	{
		int32 numParamsChanged = paramChanges->getParameterCount ();
		// for each parameter which are some changes in this audio block:
		for (int32 i = 0; i < numParamsChanged; i++)
		{
			IParamValueQueue* paramQueue = paramChanges->getParameterData (i);
			if (paramQueue)
			{
				ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
					case kGainId:
						// we use in this example only the last point of the queue.
						// in some wanted case for specific kind of parameter it makes sense to retrieve all points
						// and process the whole audio block in small blocks.
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
						{
							fGain = (float)value;
						}
						break;

					case kBypassId:
						if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
						{
							bBypass = (value > 0.5f);
						}
						break;
				}
			}
		}
	}
	
	//---2) Read input events-------------
	IEventList* eventList = data.inputEvents;
	if (eventList) 
	{
		int32 numEvent = eventList->getEventCount ();
		for (int32 i = 0; i < numEvent; i++)
		{
			Event event;
			if (eventList->getEvent (i, event) == kResultOk)
			{
				switch (event.type)
				{
					//----------------------
					case Event::kNoteOnEvent:
						// use the velocity as gain modifier
						fGainReduction = event.noteOn.velocity;
						break;
					
					//----------------------
					case Event::kNoteOffEvent:
						// noteOff reset the reduction
						fGainReduction = 0.f;
						break;
				}
			}
		}
	}
		
	//-------------------------------------
	//---3) Process Audio---------------------
	//-------------------------------------
	if (data.numInputs == 0 || data.numOutputs == 0)
	{
		// nothing to do
		return kResultOk;
	}

	// (simplification) we suppose in this example that we have the same input channel count than the output
	int32 numChannels = data.inputs[0].numChannels;

	//---get audio buffers----------------
	uint32 sampleFramesSize = getSampleFramesSizeInBytes (processSetup, data.numSamples);
	void** in = getChannelBuffersPointer (processSetup, data.inputs[0]);
	void** out = getChannelBuffersPointer (processSetup, data.outputs[0]);

	//---check if silence---------------
	// normally we have to check each channel (simplification)
	if (data.inputs[0].silenceFlags != 0)
	{
		// mark output silence too
		data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
		
		// the plug-in has to be sure that if it sets the flags silence that the output buffer are clear
		for (int32 i = 0; i < numChannels; i++)
		{
			// do not need to be cleared if the buffers are the same (in this case input buffer are already cleared by the host)
			if (in[i] != out[i])
			{				
				memset (out[i], 0, sampleFramesSize);
			}
		}

		// nothing to do at this point
		return kResultOk;
	}

	// mark our outputs has not silent
	data.outputs[0].silenceFlags = 0;

	//---in bypass mode outputs should be like inputs-----
	if (bBypass)
	{
		for (int32 i = 0; i < numChannels; i++)
		{
			// do not need to be copied if the buffers are the same
			if (in[i] != out[i])
			{
				memcpy (out[i], in[i], sampleFramesSize);
			}
		}
		// in this example we do not update the VuMeter in Bypass
	}
	else
	{
		float fVuPPM = 0.f;

		//---apply gain factor----------
		float gain = (fGain - fGainReduction);
		if (bHalfGain)
		{
			gain = gain * 0.5f;
		}
		
		// if the applied gain is nearly zero, we could say that the outputs are zeroed and we set the silence flags. 
		if (gain < 0.0000001)
		{
			for (int32 i = 0; i < numChannels; i++)
			{
				memset (out[i], 0, sampleFramesSize);
			}
			data.outputs[0].silenceFlags = (1 << numChannels) - 1;  // this will set to 1 all channels
		}
		else
		{
			if (data.symbolicSampleSize == kSample32)
				fVuPPM = processAudio<Sample32> ((Sample32**)in, (Sample32**)out, numChannels,
				                                 data.numSamples, gain);
			else
				fVuPPM = processAudio<Sample64> ((Sample64**)in, (Sample64**)out, numChannels,
				                                 data.numSamples, gain);
		}

		//---3) Write outputs parameter changes-----------
		IParameterChanges* outParamChanges = data.outputParameterChanges;
		// a new value of VuMeter will be send to the host 
		// (the host will send it back in sync to our controller for updating our editor)
		if (outParamChanges && fVuPPMOld != fVuPPM)
		{
			int32 index = 0;
			IParamValueQueue* paramQueue = outParamChanges->addParameterData (kVuPPMId, index);
			if (paramQueue)
			{
				int32 index2 = 0;
				paramQueue->addPoint (0, fVuPPM, index2); 
			}
		}
		fVuPPMOld = fVuPPM;
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::setState (IBStream* state)
{
	// we receive the current  (processor part)
	// called when we load a preset, the model has to be reloaded

	IBStreamer streamer (state, kLittleEndian);
	float savedGain = 0.f;
	if (streamer.readFloat (savedGain) == false)
		return kResultFalse;

	float savedGainReduction = 0.f;
	if (streamer.readFloat (savedGainReduction) == false)
		return kResultFalse;

	int32 savedBypass = 0;
	if (streamer.readInt32 (savedBypass) == false)
		return kResultFalse;

	fGain = savedGain;
	fGainReduction = savedGainReduction;
	bBypass = savedBypass > 0;

	setParamNormalized (kGainId, savedGain);
	setParamNormalized (kBypassId, bBypass);

	// Example of using the IStreamAttributes interface
	FUnknownPtr<IStreamAttributes> stream (state);
	if (stream)
	{
		IAttributeList* list = stream->getAttributes ();
		if (list)
		{
			// get the current type (project/Default..) of this state
			String128 string = {0};
			if (list->getString (PresetAttributes::kStateType, string, 128 * sizeof (TChar)) == kResultTrue)
			{
				UString128 tmp (string);
				char ascii[128];
				tmp.toAscii (ascii, 128);
				if (!strncmp (ascii, StateType::kProject, strlen (StateType::kProject)))
				{
					// we are in project loading context...
				}
			}

			// get the full file path of this state
			TChar fullPath[1024];
			memset (fullPath, 0, 1024 * sizeof (TChar));
			if (list->getString (PresetAttributes::kFilePathStringType, fullPath, 1024 * sizeof (TChar)) == kResultTrue)
			{
				// here we have the full path ...
			}
		}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::getState (IBStream* state)
{
	// here we need to save the model

	IBStreamer streamer (state, kLittleEndian);

	streamer.writeFloat (fGain);
	streamer.writeFloat (fGainReduction);
	streamer.writeInt32 (bBypass ? 1 : 0);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::setupProcessing (ProcessSetup& newSetup)
{
	// called before the process call, always in a disable state (not active)

	// here we keep a trace of the processing mode (offline,...) for example.
	currentProcessMode = newSetup.processMode;

	return SingleComponentEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::setBusArrangements (SpeakerArrangement* inputs, int32 numIns,
                                                    SpeakerArrangement* outputs, int32 numOuts)
{
	if (numIns == 1 && numOuts == 1)
	{
		// the host wants Mono => Mono (or 1 channel -> 1 channel)
		if (SpeakerArr::getChannelCount (inputs[0]) == 1 &&
		    SpeakerArr::getChannelCount (outputs[0]) == 1)
		{
			auto* bus = FCast<AudioBus> (audioInputs.at (0));
			if (bus)
			{
				// check if we are Mono => Mono, if not we need to recreate the busses
				if (bus->getArrangement () != inputs[0])
				{
					bus->setArrangement (inputs[0]);
					bus->setName (STR16 ("Mono In"));
					if (auto* busOut = FCast<AudioBus> (audioOutputs.at (0)))
					{
						busOut->setArrangement (inputs[0]);
						busOut->setName (STR16 ("Mono Out"));
					}
				}
				return kResultOk;
			}
		}
		// the host wants something else than Mono => Mono, in this case we are always Stereo => Stereo
		else
		{
			auto* bus = FCast<AudioBus> (audioInputs.at (0));
			if (bus)
			{
				tresult result = kResultFalse;
				
				// the host wants 2->2 (could be LsRs -> LsRs)
				if (SpeakerArr::getChannelCount (inputs[0]) == 2 && SpeakerArr::getChannelCount (outputs[0]) == 2)
				{
					bus->setArrangement (inputs[0]);
					bus->setName (STR16 ("Stereo In"));
					if (auto* busOut = FCast<AudioBus> (audioOutputs.at (0)))
					{
						busOut->setArrangement (outputs[0]);
						busOut->setName (STR16 ("Stereo Out"));
					}
					result = kResultTrue;
				}
				// the host want something different than 1->1 or 2->2 : in this case we want stereo
				else if (bus->getArrangement () != SpeakerArr::kStereo)
				{
					bus->setArrangement (SpeakerArr::kStereo);
					bus->setName (STR16 ("Stereo In"));
					if (auto* busOut = FCast<AudioBus> (audioOutputs.at (0)))
					{
						busOut->setArrangement (SpeakerArr::kStereo);
						busOut->setName (STR16 ("Stereo Out"));
					}

					result = kResultFalse;
				}

				return result;
			}
		}
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::canProcessSampleSize (int32 symbolicSampleSize)
{
	if (symbolicSampleSize == kSample32)
		return kResultTrue;

	// we support double processing
	if (symbolicSampleSize == kSample64)
		return kResultTrue;

	return kResultFalse;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API AGainSimple::createView (const char* name)
{
	// someone wants my editor
	if (name && FIDStringsEqual (name, ViewType::kEditor))
	{
		auto* view = new VST3Editor (this, "view", "again.uidesc");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::getMidiControllerAssignment (int32 busIndex,
	int16 /*midiChannel*/,
	CtrlNumber midiControllerNumber,
	ParamID& tag)
{
	// we support for the Gain parameter all MIDI Channel but only first bus (there is only one!)
	if (busIndex == 0 && midiControllerNumber == kCtrlVolume)
	{
		tag = kGainId;
		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
IController* AGainSimple::createSubController (UTF8StringPtr name,
                                               const IUIDescription* description,
                                               VST3Editor* editor)
{
	if (UTF8StringView (name) == "MessageController")
	{
		auto* controller = new UIMessageController (this);
		addUIMessageController (controller);
		return controller;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::setEditorState (IBStream* state)
{
	tresult result = kResultFalse;

	int8 byteOrder;
	if ((result = state->read (&byteOrder, sizeof (int8))) != kResultTrue)
		return result;
	if ((result = state->read (defaultMessageText, 128 * sizeof (TChar))) != kResultTrue)
		return result;

	// if the byteorder doesn't match, byte swap the text array ...
	if (byteOrder != BYTEORDER)
	{
		for (int32 i = 0; i < 128; i++)
			SWAP_16 (defaultMessageText[i])
	}

	for (auto& uiMessageController : uiMessageControllers)
		uiMessageController->setMessageText (defaultMessageText);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::getEditorState (IBStream* state)
{
	// here we can save UI settings for example

	IBStreamer streamer (state, kLittleEndian);

	// as we save a Unicode string, we must know the byteorder when setState is called
	int8 byteOrder = BYTEORDER;
	if (streamer.writeInt8 (byteOrder) == false)
		return kResultFalse;

	if (streamer.writeRaw (defaultMessageText, 128 * sizeof (TChar)) == false)
		return kResultFalse;

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::setParamNormalized (ParamID tag, ParamValue value)
{
	// called from host to update our parameters state
	tresult result = SingleComponentEffect::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::getParamStringByValue (ParamID tag, ParamValue valueNormalized,
                                                       String128 string)
{
	return SingleComponentEffect::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::getParamValueByString (ParamID tag, TChar* string,
                                                       ParamValue& valueNormalized)
{
	return SingleComponentEffect::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
void AGainSimple::addUIMessageController (UIMessageController* controller)
{
	uiMessageControllers.push_back (controller);
}

//------------------------------------------------------------------------
void AGainSimple::removeUIMessageController (UIMessageController* controller)
{
	UIMessageControllerList::const_iterator it =
	    std::find (uiMessageControllers.begin (), uiMessageControllers.end (), controller);
	if (it != uiMessageControllers.end ())
		uiMessageControllers.erase (it);
}

//------------------------------------------------------------------------
void AGainSimple::setDefaultMessageText (String128 text)
{
	UString str (defaultMessageText, 128);
	str.assign (text, -1);
}

//------------------------------------------------------------------------
TChar* AGainSimple::getDefaultMessageText ()
{
	return defaultMessageText;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API AGainSimple::queryInterface (const TUID iid, void** obj)
{
	DEF_INTERFACE (IMidiMapping)
	return SingleComponentEffect::queryInterface (iid, obj);
}

//------------------------------------------------------------------------
enum
{
	// UI size
	kEditorWidth = 350,
	kEditorHeight = 120
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// called when library is loaded
bool InitModule ()
{
	return true;
}

//------------------------------------------------------------------------
// called when library is unloaded
bool DeinitModule ()
{
	return true;
}

//------------------------------------------------------------------------
BEGIN_FACTORY_DEF ("Steinberg Media Technologies", 
				   "http://www.steinberg.net", 
				   "mailto:info@steinberg.de")

	//---First plug-in included in this factory-------
	// its kVstAudioEffectClass component
	DEF_CLASS2 (INLINE_UID (0xB9F9ADE1, 0xCD9C4B6D, 0xA57E61E3, 0x123535FD),
				PClassInfo::kManyInstances,					// cardinality  
				kVstAudioEffectClass,						// the component category (do not changed this)
				"AGainSimple VST3",							// here the plug-in name (to be changed)
				0,											// single component effects cannot be distributed so this is zero
				"Fx",										// Subcategory for this plug-in (to be changed)
				FULL_VERSION_STR,							// Plug-in version (to be changed)
				kVstVersionString,							// the VST 3 SDK version (do not changed this, use always this define)
				Steinberg::Vst::AGainSimple::createInstance)// function pointer called when this component should be instantiated

END_FACTORY
