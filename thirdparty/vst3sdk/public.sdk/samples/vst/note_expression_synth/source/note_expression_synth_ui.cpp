//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_ui.cpp
// Created by  : Steinberg, 12/2010
// Description : Note Expression Synth UI version using VSTGUI 4
// Flags       : clang-format SMTGSequencer
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

#include "note_expression_synth_ui.h"
#include "vstgui/contrib/keyboardview.h"
#include "vstgui/plugin-bindings/vst3groupcontroller.h"
#include "vstgui/plugin-bindings/vst3padcontroller.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstinterappaudio.h"
#include "pluginterfaces/vst/ivstpluginterfacesupport.h"

using namespace VSTGUI;

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace NoteExpressionSynth {

//------------------------------------------------------------------------
class ConditionalRemoveViewController : public DelegationController
{
public:
	ConditionalRemoveViewController (IController* controller, bool needed)
	: DelegationController (controller), needed (needed)
	{
	}

	CView* verifyView (CView* view, const UIAttributes& attributes,
	                   const IUIDescription* description) override
	{
		if (!needed)
		{
			view->forget ();
			return nullptr;
		}
		return controller->verifyView (view, attributes, description);
	}
	bool needed;
};

//------------------------------------------------------------------------
class InterAppAudioControlsController : public IController
{
public:
	enum
	{
		kSettings = 5000,
		kLoadPreset,
		kSavePreset
	};

	InterAppAudioControlsController (IInterAppAudioHost* iaaHost) : host (iaaHost) {}

	void valueChanged (VSTGUI::CControl* pControl) override
	{
		if (pControl->getValue ())
		{
			switch (pControl->getTag ())
			{
				case kSettings:
				{
					host->showSettingsView ();
					break;
				}
				case kLoadPreset:
				{
					getPresetManager ()->runLoadPresetBrowser ();
					break;
				}
				case kSavePreset:
				{
					getPresetManager ()->runSavePresetBrowser ();
					break;
				}
			}
		}
	}

private:
	IInterAppAudioPresetManager* getPresetManager ()
	{
		if (presetManager == nullptr)
		{
			TUID uid;
			ProcessorWithUIController::cid.toTUID (uid);
			presetManager = owned (host->createPresetManager (uid));
		}
		return presetManager;
	}
	IInterAppAudioHost* host;
	IPtr<IInterAppAudioPresetManager> presetManager;
};

//------------------------------------------------------------------------
class InterAppAudioPlayer : public KeyboardViewPlayerDelegate
{
public:
	InterAppAudioPlayer (IInterAppAudioHost* _host) : host (_host) {}

	int32_t onNoteOn (NoteIndex note, double xPos, double yPos) override
	{
		Event e = {};
		e.type = Event::kNoteOnEvent;
		e.noteOn.pitch = note;
		e.noteOn.velocity = static_cast<float> (yPos);
		if (host->scheduleEventFromUI (e) == kResultTrue)
		{
			onNoteModulation (e.noteOn.noteId, xPos, yPos);
			return e.noteOn.noteId;
		}
		return note;
	}
	void onNoteOff (NoteIndex note, int32_t noteID) override
	{
		Event e = {};
		e.type = Event::kNoteOffEvent;
		e.noteOff.noteId = noteID;
		e.noteOff.pitch = note;
		e.noteOff.velocity = 0.f;
		host->scheduleEventFromUI (e);
	}
	void onNoteModulation (int32_t noteID, double xPos, double yPos) override
	{
		Event e = {};
		e.type = Event::kNoteExpressionValueEvent;
		e.noteExpressionValue.noteId = noteID;
		e.noteExpressionValue.typeId = noteExpressionModulationID;
		e.noteExpressionValue.value = xPos;
		host->scheduleEventFromUI (e);
		e.noteExpressionValue.typeId = kVolumeTypeID;
		e.noteExpressionValue.value = yPos;
		host->scheduleEventFromUI (e);
	}

private:
	IInterAppAudioHost* host {nullptr};
	NoteExpressionTypeID noteExpressionModulationID {Controller::kFilterFreqModTypeID};
};

//------------------------------------------------------------------------
class KeyboardController : public DelegationController,
                           public ViewListenerAdapter,
                           public IKeyboardViewKeyRangeChangedListener,
                           public KeyboardViewPlayerDelegate
{
public:
	KeyboardController (IController* parent, IKeyboardViewPlayerDelegate* player,
	                    KeyboardViewRangeSelector::Range& range)
	: DelegationController (parent), player (player), selectedRange (range)
	{
	}
	~KeyboardController () noexcept override
	{
		if (keyboard)
			keyboard->unregisterViewListener (this);
		if (rangeSelector)
		{
			rangeSelector->unregisterViewListener (this);
			rangeSelector->unregisterKeyRangeChangedListener (this);
		}
	}

	CView* verifyView (CView* view, const UIAttributes& attributes,
	                   const IUIDescription* description) override
	{
		if (auto kb = dynamic_cast<KeyboardView*> (view))
		{
			assert (keyboard == nullptr);
			keyboard = kb;
			keyboard->registerViewListener (this);
			keyboard->setDelegate (this);
		}
		else if (auto kbsv = dynamic_cast<KeyboardViewRangeSelector*> (view))
		{
			assert (rangeSelector == nullptr);
			rangeSelector = kbsv;
			rangeSelector->registerViewListener (this);
			rangeSelector->registerKeyRangeChangedListener (this);
			if (selectedRange.length > 0)
				rangeSelector->setSelectionRange (selectedRange);
		}
		return controller->verifyView (view, attributes, description);
	}
	void viewAttached (CView* view) override
	{
		if (view == rangeSelector)
		{
			updateKeyboard ();
		}
	}
	void viewWillDelete (CView* view) override
	{
		if (view == rangeSelector)
		{
			rangeSelector = nullptr;
		}
		else if (view == keyboard)
		{
			keyboard = nullptr;
		}
		view->unregisterViewListener (this);
	}

private:
	int32_t onNoteOn (NoteIndex note, double xPos, double yPos) override
	{
		int32_t noteID = note;
		if (player)
			noteID = player->onNoteOn (note, xPos, yPos);
		keyboard->setKeyPressed (note, true);
		rangeSelector->setKeyPressed (note, true);
		return noteID;
	}
	void onNoteOff (NoteIndex note, int32_t noteID) override
	{
		if (player)
			player->onNoteOff (note, noteID);
		rangeSelector->setKeyPressed (note, false);
		keyboard->setKeyPressed (note, false);
	}
	void onNoteModulation (int32_t noteID, double xPos, double yPos) override
	{
		if (player)
			player->onNoteModulation (noteID, xPos, yPos);
	}
	void onKeyRangeChanged (KeyboardViewRangeSelector*) override
	{
		if (!keyboard)
			return;
		auto range = rangeSelector->getSelectionRange ();
		while (!keyboard->isWhiteKey (range.position))
			range.position--;
		rangeSelector->setSelectionRange (range);
		updateKeyboard ();
		selectedRange = rangeSelector->getSelectionRange ();
	}

	void updateKeyboard ()
	{
		if (keyboard && rangeSelector)
		{
			auto range = rangeSelector->getSelectionRange ();
			CCoord whiteKeyWidth = std::floor (keyboard->getViewSize ().getWidth () /
			                                   rangeSelector->getNumWhiteKeysSelected ());
			if (range.position + range.length >
			    rangeSelector->getNumKeys () + rangeSelector->getKeyRangeStart ())
			{
				range.length -= 1;
				rangeSelector->setSelectionRange (range);
			}
			keyboard->setKeyRange (range.position, range.length);
			keyboard->setWhiteKeyWidth (whiteKeyWidth);
			keyboard->setBlackKeyWidth (whiteKeyWidth / 1.5);
			keyboard->setBlackKeyHeight (keyboard->getHeight () / 2.);
		}
	}

	KeyboardView* keyboard {nullptr};
	KeyboardViewRangeSelector* rangeSelector {nullptr};
	IKeyboardViewPlayerDelegate* player {nullptr};
	KeyboardViewRangeSelector::Range& selectedRange;
};

//------------------------------------------------------------------------
FUID ControllerWithUI::cid (0x1AA302B3, 0xE8384785, 0xB9C3FE3E, 0x08B056F5);
FUID ProcessorWithUIController::cid (0x41466D9B, 0xB0654576, 0xB641098F, 0x686371B3);

enum
{
	kParamMIDILearn = kNumGlobalParameters,
	kParamEnableMPE
};

//------------------------------------------------------------------------
tresult PLUGIN_API ControllerWithUI::initialize (FUnknown* context)
{
	auto result = Controller::initialize (context);
	if (result == kResultTrue)
	{
		parameters.addParameter (USTRING ("MIDI Learn"), nullptr, 1, 0, ParameterInfo::kCanAutomate,
		                         kParamMIDILearn);
		FUnknownPtr<IVst3WrapperMPESupport> mpeSupport (context);
		bool addEnableMPE = mpeSupport;
#if DEVELOPMENT
		addEnableMPE = true;
#endif
		if (addEnableMPE)

		{
			parameters.addParameter (USTRING ("Enable MPE"), nullptr, 1, 0,
			                         ParameterInfo::kCanAutomate, kParamEnableMPE);
		}
	}
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ControllerWithUI::terminate ()
{
	if (playerDelegate)
		delete playerDelegate;
	playerDelegate = nullptr;
	return Controller::terminate ();
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API ControllerWithUI::createView (FIDString _name)
{
	ConstString name (_name);
	if (name == ViewType::kEditor)
	{
		FUnknownPtr<IInterAppAudioHost> interAudioApp (getHostContext ());
		if (interAudioApp)
		{
			ViewRect vr;
			float scale;
			if (interAudioApp->getScreenSize (&vr, &scale) == kResultTrue)
			{
				if (vr.right >= 1024)
				{
					return new VST3Editor (this, "EditorIPad", "note_expression_synth.uidesc");
				}
				else if (vr.right == 812)
				{
					return new VST3Editor (this, "EditorIPhoneX", "note_expression_synth.uidesc");
				}
				else
				{
					return new VST3Editor (this, "EditorIPhone4Inch",
					                       "note_expression_synth.uidesc");
				}
			}
		}
		FUnknownPtr<IVst3ToAUWrapper> auWrapper (getHostContext ());
		FUnknownPtr<IVst3WrapperMPESupport> mpeSupport (getHostContext ());
		if (auWrapper && mpeSupport)
			return new VST3Editor (this, "EditorIPad_AUv3", "note_expression_synth.uidesc");
		return new VST3Editor (this, "Editor", "note_expression_synth.uidesc");
	}
	return nullptr;
}

//------------------------------------------------------------------------
IController* ControllerWithUI::createSubController (UTF8StringPtr _name,
                                                    const IUIDescription* description,
                                                    VST3Editor* editor)
{
	ConstString name (_name);
	if (name == "FilterPadController")
	{
		Parameter* freqParam = getParameterObject (kParamFilterFreq);
		Parameter* resoParam = getParameterObject (kParamFilterQ);
		auto* padController = new PadController (editor, this, freqParam, resoParam);
		return padController;
	}
	else if (name == "FilterTypeController")
	{
		auto* controller = new GroupController (getParameterObject (kParamFilterType), this);
		return controller;
	}
	else if (name == "InterAppAudioControlsController")
	{
		FUnknownPtr<IInterAppAudioHost> interAudioApp (getHostContext ());
		if (interAudioApp)
		{
			return new InterAppAudioControlsController (interAudioApp);
		}
	}
	else if (name == "KeyboardController")
	{
		if (playerDelegate == nullptr)
		{
			FUnknownPtr<IInterAppAudioHost> interAudioApp (getHostContext ());
			if (interAudioApp)
			{
				playerDelegate = new InterAppAudioPlayer (interAudioApp);
			}
		}
		return new KeyboardController (editor, playerDelegate, keyboardRange);
	}
	else if (name == "MPEController")
	{
#if DEVELOPMENT
		// make sure in DEVELOPMENT mode not to remove the UI
		bool mpeSupportNeeded = true;
#else
		FUnknownPtr<IVst3WrapperMPESupport> mpeSupport (getHostContext ());
		bool mpeSupportNeeded = mpeSupport ? true : false;
#endif
		return new ConditionalRemoveViewController (editor, mpeSupportNeeded);
	}
	else if (name == "MidiLearnController")
	{
#if DEVELOPMENT
		// make sure in DEVELOPMENT mode not to remove the UI
		bool midiLearnSupported = true;
#else
		bool midiLearnSupported = false;
		FUnknownPtr<IPlugInterfaceSupport> pis (getHostContext ());
		if (pis)
			midiLearnSupported = pis->isPlugInterfaceSupported (IMidiLearn::iid) == kResultTrue;
#endif
		return new ConditionalRemoveViewController (editor, midiLearnSupported);
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ControllerWithUI::setState (IBStream* state)
{
	IBStreamer stream (state, kLittleEndian);
	uint8 value;
	if (stream.readInt8u (value))
		keyboardRange.position = value;
	if (stream.readInt8u (value))
		keyboardRange.length = value;
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ControllerWithUI::getState (IBStream* state)
{
	IBStreamer stream (state, kLittleEndian);
	if (!stream.writeInt8u (keyboardRange.position))
		return kInternalError;
	if (!stream.writeInt8u (keyboardRange.length))
		return kInternalError;
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult ControllerWithUI::beginEdit (ParamID tag)
{
	if (tag >= kParamMIDILearn)
	{
		return kResultTrue;
	}
	if (doMIDILearn)
		midiLearnParamID = tag;
	return Controller::beginEdit (tag);
}

//------------------------------------------------------------------------
tresult ControllerWithUI::performEdit (ParamID tag, ParamValue valueNormalized)
{
	if (tag == kParamMIDILearn)
	{
		doMIDILearn = valueNormalized > 0.5;
		if (doMIDILearn)
			midiLearnParamID = InvalidParamID;
		return kResultTrue;
	}
	else if (tag == kParamEnableMPE)
	{
		FUnknownPtr<IVst3WrapperMPESupport> mpeSupport (getHostContext ());
		if (mpeSupport)
		{
			mpeSupport->enableMPEInputProcessing (valueNormalized < 0.5);
		}
		return kResultTrue;
	}
	return Controller::performEdit (tag, valueNormalized);
}

//------------------------------------------------------------------------
tresult ControllerWithUI::endEdit (ParamID tag)
{
	if (tag >= kParamMIDILearn)
	{
		return kResultTrue;
	}
	return Controller::endEdit (tag);
}

//------------------------------------------------------------------------
tresult PLUGIN_API ControllerWithUI::onLiveMIDIControllerInput (int32 busIndex, int16 channel,
                                                                CtrlNumber midiCC)
{
	if (!doMIDILearn || busIndex != 0 || channel != 0 || midiLearnParamID == InvalidParamID)
		return kResultFalse;
	if (midiCCMapping[midiCC] != midiLearnParamID)
	{
		for (auto&& pid : midiCCMapping)
		{
			if (pid == midiLearnParamID)
				pid = InvalidParamID;
		}
		midiCCMapping[midiCC] = midiLearnParamID;
		if (auto componentHandler = getComponentHandler ())
		{
			componentHandler->restartComponent (kMidiCCAssignmentChanged);
		}
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
bool ControllerWithUI::isPrivateParameter (const ParamID paramID)
{
	return paramID >= kParamMIDILearn;
}

//------------------------------------------------------------------------
ProcessorWithUIController::ProcessorWithUIController ()
{
	setControllerClass (ControllerWithUI::cid);
}

//------------------------------------------------------------------------
} // NoteExpressionSynth
} // Vst
} // Steinberg
