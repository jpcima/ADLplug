//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_ui.h
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

#pragma once

#include "note_expression_synth_controller.h"
#include "note_expression_synth_processor.h"
#include "vstgui/contrib/keyboardview.h"
#include "vstgui/plugin-bindings/vst3editor.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace NoteExpressionSynth {

//-----------------------------------------------------------------------------
/** Example Note Expression Audio Controller + User Interface */
class ControllerWithUI : public Controller, public IMidiLearn, public VSTGUI::VST3EditorDelegate
{
public:
	using UTF8StringPtr = VSTGUI::UTF8StringPtr;
	using IController = VSTGUI::IController;
	using IUIDescription = VSTGUI::IUIDescription;
	using VST3Editor = VSTGUI::VST3Editor; 
	
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	IPlugView* PLUGIN_API createView (FIDString name) SMTG_OVERRIDE;
	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;
	tresult beginEdit (ParamID tag) SMTG_OVERRIDE;
	tresult performEdit (ParamID tag, ParamValue valueNormalized) SMTG_OVERRIDE;
	tresult endEdit (ParamID tag) SMTG_OVERRIDE;

	//--- IMidiLearn ---------------------------------
	tresult PLUGIN_API onLiveMIDIControllerInput (int32 busIndex, int16 channel,
	                                              CtrlNumber midiCC) SMTG_OVERRIDE;

	// VST3EditorDelegate
	IController* createSubController (UTF8StringPtr name, const IUIDescription* description,
	                                  VST3Editor* editor) SMTG_OVERRIDE;
	bool isPrivateParameter (const ParamID paramID) SMTG_OVERRIDE;
	
	static FUnknown* createInstance (void*) { return (IEditController*)new ControllerWithUI (); }

	static FUID cid;

	DEFINE_INTERFACES
		DEF_INTERFACE (IMidiLearn)
	END_DEFINE_INTERFACES (Controller)
	REFCOUNT_METHODS (Controller)

private:
	VSTGUI::IKeyboardViewPlayerDelegate* playerDelegate {nullptr};
	VSTGUI::KeyboardViewRangeSelector::Range keyboardRange {};
	ParamID midiLearnParamID {InvalidParamID};
	bool doMIDILearn {false};
};

//-----------------------------------------------------------------------------
/** Example Note Expression Audio Processor + User Interface */
class ProcessorWithUIController : public Processor
{
public:
	ProcessorWithUIController ();

	static FUnknown* createInstance (void*)
	{
		return (IAudioProcessor*)new ProcessorWithUIController ();
	}

	static FUID cid;
};

//------------------------------------------------------------------------
} // NoteExpressionSynth
} // Vst
} // Steinberg
