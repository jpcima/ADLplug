//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/again/source/againcontroller.h
// Created by  : Steinberg, 04/2005
// Description : AGain Editor Example for VST 3
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

#include "vstgui/plugin-bindings/vst3editor.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#include <vector>

namespace Steinberg {
namespace Vst {

template <typename T>
class AGainUIMessageController;

//------------------------------------------------------------------------
// AGainController
//------------------------------------------------------------------------
class AGainController : public EditControllerEx1, public IMidiMapping, public VSTGUI::VST3EditorDelegate
{
public:
	using UIMessageController = AGainUIMessageController<AGainController>;
	using UTF8StringPtr = VSTGUI::UTF8StringPtr;
	using IUIDescription = VSTGUI::IUIDescription;
	using IController = VSTGUI::IController;
	using VST3Editor = VSTGUI::VST3Editor;
	
	//--- ---------------------------------------------------------------------
	// create function required for plug-in factory,
	// it will be called to create new instances of this controller
	//--- ---------------------------------------------------------------------
	static FUnknown* createInstance (void* /*context*/)
	{
		return (IEditController*)new AGainController;
	}

	//---from IPluginBase--------
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API terminate () SMTG_OVERRIDE;

	//---from EditController-----
	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;
	IPlugView* PLUGIN_API createView (const char* name) SMTG_OVERRIDE;
	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API setParamNormalized (ParamID tag, ParamValue value) SMTG_OVERRIDE;
	tresult PLUGIN_API getParamStringByValue (ParamID tag, ParamValue valueNormalized,
	                                          String128 string) SMTG_OVERRIDE;
	tresult PLUGIN_API getParamValueByString (ParamID tag, TChar* string,
	                                          ParamValue& valueNormalized) SMTG_OVERRIDE;

	//---from ComponentBase-----
	tresult receiveText (const char* text) SMTG_OVERRIDE;

	//---from IMidiMapping-----------------
	tresult PLUGIN_API getMidiControllerAssignment (int32 busIndex, int16 channel,
	                                                CtrlNumber midiControllerNumber,
	                                                ParamID& tag) SMTG_OVERRIDE;

	//---from VST3EditorDelegate-----------
	IController* createSubController (UTF8StringPtr name, const IUIDescription* description,
	                                  VST3Editor* editor) SMTG_OVERRIDE;

	DELEGATE_REFCOUNT (EditController)
	tresult PLUGIN_API queryInterface (const char* iid, void** obj) SMTG_OVERRIDE;

	//---Internal functions-------
	void addUIMessageController (UIMessageController* controller);
	void removeUIMessageController (UIMessageController* controller);

	void setDefaultMessageText (String128 text);
	TChar* getDefaultMessageText ();
//------------------------------------------------------------------------

private:
	using UIMessageControllerList = std::vector<UIMessageController*>;
	UIMessageControllerList uiMessageControllers;

	String128 defaultMessageText;
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
