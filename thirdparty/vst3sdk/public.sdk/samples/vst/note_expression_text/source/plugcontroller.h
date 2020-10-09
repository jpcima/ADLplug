//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/again/source/plugcontroller.h
// Created by  : Steinberg, 04/2005
// Description : Note Expression Editor Example for VST 3
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

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "vstgui/plugin-bindings/vst3editor.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// PlugController
//------------------------------------------------------------------------
class PlugController : public EditControllerEx1,
                       public VSTGUI::VST3EditorDelegate,
                       public INoteExpressionController
{
public:
//------------------------------------------------------------------------
// create function required for plug-in factory,
// it will be called to create new instances of this controller
//------------------------------------------------------------------------
	static FUnknown* createInstance (void* context)
	{
		return (IEditController*)new PlugController;
	}

	using CView = VSTGUI::CView;
	using CTextEdit = VSTGUI::CTextEdit;
	using UTF8StringPtr = VSTGUI::UTF8StringPtr;
	using UIAttributes = VSTGUI::UIAttributes;
	using IUIDescription = VSTGUI::IUIDescription;
	using VST3Editor = VSTGUI::VST3Editor;
	using CTextLabel = VSTGUI::CTextLabel;

	//---from IPluginBase--------
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	
	//---from EditController-----
	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;

	IPlugView* PLUGIN_API createView (const char* name) SMTG_OVERRIDE;

	//---from VST3EditorDelegate----
	CView* createCustomView (UTF8StringPtr name, const UIAttributes& attributes,
	                         const IUIDescription* description, VST3Editor* editor) SMTG_OVERRIDE;
	void willClose (VST3Editor* editor) SMTG_OVERRIDE;

	tresult PLUGIN_API notify (IMessage* message) SMTG_OVERRIDE;
	tresult receiveText (const char8* text) SMTG_OVERRIDE;

	//---from INoteExpressionController--------
	int32 PLUGIN_API getNoteExpressionCount (int32 busIndex, int16 channel) SMTG_OVERRIDE;
	tresult PLUGIN_API getNoteExpressionInfo (int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo& info /*out*/) SMTG_OVERRIDE;
	tresult PLUGIN_API getNoteExpressionStringByValue (int32 busIndex, int16 channel, NoteExpressionTypeID id, NoteExpressionValue valueNormalized /*in*/, String128 string /*out*/) SMTG_OVERRIDE;
	tresult PLUGIN_API getNoteExpressionValueByString (int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar* string /*in*/, NoteExpressionValue& valueNormalized /*out*/) SMTG_OVERRIDE;

	//---Interface---------
	OBJ_METHODS (PlugController, EditControllerEx1)
	DEFINE_INTERFACES
		DEF_INTERFACE (INoteExpressionController)
	END_DEFINE_INTERFACES (EditControllerEx1)
	REFCOUNT_METHODS(EditControllerEx1)

	//------------------------------------------------------------------------

private:
	CTextLabel* mTextLabel;
};

}} // namespaces
