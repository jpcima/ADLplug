//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/hostcheckercontroller.h
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

#include "eventlogdatabrowsersource.h"
#include "public.sdk/source/common/threadchecker.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#include "hostcheck.h"
#include "logevents.h"
#include "vstgui/lib/cvstguitimer.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "base/source/fstring.h"
#include "pluginterfaces/vst/ivstautomationstate.h"
#include "pluginterfaces/vst/ivstchannelcontextinfo.h"
#include "pluginterfaces/vst/ivstmidilearn.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "pluginterfaces/vst/ivstparameterfunctionname.h"
#include "pluginterfaces/vst/ivstphysicalui.h"
#include "pluginterfaces/vst/ivstprefetchablesupport.h"
#include "pluginterfaces/vst/ivstrepresentation.h"

namespace Steinberg {

namespace HostChecker {
const int32 kMaxLatency = 8192;
const uint32 kParamWarnCount = 7;
const uint32 kParamWarnBitCount = 24;
const uint32 kParamWarnStepCount = 1 << kParamWarnBitCount;
}

namespace Vst {

enum
{
	// for Parameters
	kParam1Tag = 1000,
	kGeneratePeaksTag,
	kLatencyTag,
	kBypassTag,
	kCanResizeTag,
	kScoreTag,
	kParamWhichCouldBeHiddenTag,
	kTriggerHiddenTag,
	kTriggerProgressTag,
	kProgressValueTag,

	kProcessWarnTag,
	kLastTag = kProcessWarnTag + HostChecker::kParamWarnCount,

	// for Units
	kUnitId = 1234,
	kUnit2Id = 1235
};

class EditorSizeController;

//-----------------------------------------------------------------------------
class HostCheckerController : public EditControllerEx1,
                              public VSTGUI::VST3EditorDelegate,
                              public ChannelContext::IInfoListener,
                              public IXmlRepresentationController,
                              public IAutomationState,
                              public IEditControllerHostEditing,
                              public IMidiMapping,
                              public IMidiLearn,
                              public INoteExpressionController,
                              public INoteExpressionPhysicalUIMapping,
                              public IKeyswitchController,
                              public IParameterFunctionName
{
public:
	using UTF8StringPtr = VSTGUI::UTF8StringPtr;
	using IController = VSTGUI::IController;
	using IUIDescription = VSTGUI::IUIDescription;
	using VST3Editor = VSTGUI::VST3Editor;

	HostCheckerController ();

	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API terminate () SMTG_OVERRIDE;

	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getUnitByBus (MediaType type, BusDirection dir, int32 busIndex,
	                                 int32 channel, UnitID& unitId /*out*/) SMTG_OVERRIDE;
	tresult PLUGIN_API setComponentHandler (IComponentHandler* handler) SMTG_OVERRIDE;
	int32 PLUGIN_API getUnitCount () SMTG_OVERRIDE;
	tresult PLUGIN_API setParamNormalized (ParamID tag, ParamValue value) SMTG_OVERRIDE;

	tresult beginEdit (ParamID tag) SMTG_OVERRIDE;
	tresult endEdit (ParamID tag) SMTG_OVERRIDE;

	IPlugView* PLUGIN_API createView (FIDString name) SMTG_OVERRIDE;
	tresult PLUGIN_API notify (IMessage* message) SMTG_OVERRIDE;
	tresult PLUGIN_API connect (IConnectionPoint* other) SMTG_OVERRIDE;

	//---from VST3EditorDelegate --------------------
	VSTGUI::CView* createCustomView (VSTGUI::UTF8StringPtr name,
	                                 const VSTGUI::UIAttributes& attributes,
	                                 const VSTGUI::IUIDescription* description,
	                                 VSTGUI::VST3Editor* editor) SMTG_OVERRIDE;
	void willClose (VSTGUI::VST3Editor* editor) SMTG_OVERRIDE;

	//---from IEditController2-------
	tresult PLUGIN_API setKnobMode (KnobMode mode) SMTG_OVERRIDE;
	tresult PLUGIN_API openHelp (TBool /*onlyCheck*/) SMTG_OVERRIDE;
	tresult PLUGIN_API openAboutBox (TBool /*onlyCheck*/) SMTG_OVERRIDE;

	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;

	//---ChannelContext::IInfoListener-------
	tresult PLUGIN_API setChannelContextInfos (IAttributeList* list) SMTG_OVERRIDE;

	//---IXmlRepresentationController--------
	tresult PLUGIN_API getXmlRepresentationStream (RepresentationInfo& info /*in*/,
	                                               IBStream* stream /*out*/) SMTG_OVERRIDE;

	//---IMidiMapping---------------------------
	tresult PLUGIN_API getMidiControllerAssignment (int32 busIndex, int16 channel,
	                                                CtrlNumber midiControllerNumber,
	                                                ParamID& id /*out*/) SMTG_OVERRIDE;

	//---IMidiLearn-----------------------------
	tresult PLUGIN_API onLiveMIDIControllerInput (int32 busIndex, int16 channel,
	                                              CtrlNumber midiCC) SMTG_OVERRIDE;

	//---INoteExpressionController----------------------
	int32 PLUGIN_API getNoteExpressionCount (int32 busIndex, int16 channel) SMTG_OVERRIDE;
	tresult PLUGIN_API getNoteExpressionInfo (int32 busIndex, int16 channel,
	                                          int32 noteExpressionIndex,
	                                          NoteExpressionTypeInfo& info /*out*/) SMTG_OVERRIDE;
	tresult PLUGIN_API getNoteExpressionStringByValue (int32 busIndex, int16 channel,
	                                                   NoteExpressionTypeID id,
	                                                   NoteExpressionValue valueNormalized /*in*/,
	                                                   String128 string /*out*/) SMTG_OVERRIDE;
	tresult PLUGIN_API getNoteExpressionValueByString (
	    int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar* string /*in*/,
	    NoteExpressionValue& valueNormalized /*out*/) SMTG_OVERRIDE;

	//---INoteExpressionPhysicalUIMapping-----------------
	tresult PLUGIN_API getPhysicalUIMapping (int32 busIndex, int16 channel,
	                                         PhysicalUIMapList& list) SMTG_OVERRIDE;

	//--- IKeyswitchController ---------------------------
	int32 PLUGIN_API getKeyswitchCount (int32 busIndex, int16 channel) SMTG_OVERRIDE;
	tresult PLUGIN_API getKeyswitchInfo (int32 busIndex, int16 channel, int32 keySwitchIndex,
	                                     KeyswitchInfo& info /*out*/) SMTG_OVERRIDE;

	//---IAutomationState---------------------------------
	tresult PLUGIN_API setAutomationState (int32 state) SMTG_OVERRIDE;

	//---IEditControllerHostEditing-----------------------
	tresult PLUGIN_API beginEditFromHost (ParamID paramID) SMTG_OVERRIDE;
	tresult PLUGIN_API endEditFromHost (ParamID paramID) SMTG_OVERRIDE;

	//---IParameterFunctionName---------------------------
	tresult PLUGIN_API getParameterIDFromFunctionName (UnitID unitID, FIDString functionName,
	                                                   ParamID& paramID) SMTG_OVERRIDE;
	//--- --------------------------------------------------------------------------
	void editorAttached (EditorView* editor) SMTG_OVERRIDE;
	void editorRemoved (EditorView* editor) SMTG_OVERRIDE;
	void editorDestroyed (EditorView* editor) SMTG_OVERRIDE;

	IController* createSubController (UTF8StringPtr name, const IUIDescription* description,
	                                  VST3Editor* editor) override;

	tresult PLUGIN_API queryInterface (const Steinberg::TUID iid, void** obj) override;

	REFCOUNT_METHODS (EditControllerEx1)

	static FUnknown* createInstance (void*)
	{
		return (IEditController*)new HostCheckerController ();
	}

	void addFeatureLog (int32 iD, int32 count = 1, bool addToLastCount = true);
	bool getSavedSize (ViewRect& size) const
	{
		if (sizeFactor <= 0)
			return false;
		ViewRect rect (0, 0, width, height);
		size = rect;
		return true;
	}

protected:
	void extractCurrentInfo (EditorView* editor);
	float updateScoring (int32 iD);
	void onProgressTimer (VSTGUI::CVSTGUITimer*);

	std::map<VSTGUI::VST3Editor*, VSTGUI::SharedPointer<VSTGUI::CDataBrowser>> mDataBrowserMap;
	VSTGUI::SharedPointer<VSTGUI::EventLogDataBrowserSource> mDataSource;

	bool mLatencyInEdit {false};
	ParamValue mWantedLatency {0.0};

	using EditorVector = std::vector<Steinberg::Vst::EditorView*>;
	EditorVector editors;

	using EditorMap = std::map<Steinberg::Vst::EditorView*, EditorSizeController*>;
	EditorMap editorsSubCtlerMap;

	uint32 width {0};
	uint32 height {0};
	double sizeFactor {0};
	int32 inEditFromHost {0};

	std::unique_ptr<ThreadChecker> threadChecker {ThreadChecker::create ()};

	struct ScoreEntry
	{
		ScoreEntry (float factor = 1) : factor (factor) {}
		float factor {1};
		bool use {false};
	};

	using ScoreMap = std::map<uint32, ScoreEntry>;
	ScoreMap mScoreMap;

	VSTGUI::CVSTGUITimer* mProgressTimer {nullptr};
	IProgress::ID mProgressID;
	bool mInProgress {false};
};

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
