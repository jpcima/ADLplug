//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/hostchecker.cpp
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

#include "hostcheckercontroller.h"

#include "cids.h"
#include "editorsizecontroller.h"
#include "eventlogdatabrowsersource.h"
#include "hostcheckerprocessor.h"
#include "logevents.h"
#include "base/source/fstreamer.h"

#include "public.sdk/source/vst/vstcomponentbase.h"
#include "public.sdk/source/vst/vstrepresentation.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstcontextmenu.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstpluginterfacesupport.h"

#define THREAD_CHECK_MSG(msg) "The host called '" msg "' in the wrong thread context.\n"

using namespace VSTGUI;

#if DEBUG
bool THREAD_CHECK_EXIT = false;
#else
bool THREAD_CHECK_EXIT = true;
#endif

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
class MyVST3Editor : public VST3Editor
{
public:
	MyVST3Editor (HostCheckerController* controller, UTF8StringPtr templateName,
	              UTF8StringPtr xmlFile);

	void canResize (bool val) { mCanResize = val; }

protected:
	~MyVST3Editor () override;

	bool PLUGIN_API open (void* parent, const PlatformType& type) override;
	void PLUGIN_API close () override;

	bool beforeSizeChange (const CRect& newSize, const CRect& oldSize) override;

	tresult PLUGIN_API onSize (ViewRect* newSize) override;
	tresult PLUGIN_API canResize () override;
	tresult PLUGIN_API checkSizeConstraint (ViewRect* rect) override;
	tresult PLUGIN_API onKeyDown (char16 key, int16 keyMsg, int16 modifiers) override;
	tresult PLUGIN_API onKeyUp (char16 key, int16 keyMsg, int16 modifiers) override;
	tresult PLUGIN_API onWheel (float distance) override;
	tresult PLUGIN_API onFocus (TBool /*state*/) override;
	tresult PLUGIN_API setFrame (IPlugFrame* frame) override;
	tresult PLUGIN_API attached (void* parent, FIDString type) override;
	tresult PLUGIN_API removed () override;

	tresult PLUGIN_API setContentScaleFactor (ScaleFactor factor) override;

	// IParameterFinder
	tresult PLUGIN_API findParameter (int32 xPos, int32 yPos, ParamID& resultTag) override;

	void valueChanged (CControl* pControl) override;

	//---from CBaseObject---------------
	CMessageResult notify (CBaseObject* sender, const char* message) SMTG_OVERRIDE;

private:
	CVSTGUITimer* checkTimer {nullptr};

	HostCheckerController* hostController {nullptr};

	uint32 openCount = 0;
	bool wasAlreadyClosed = false;
	bool onSizeWanted = false;
	bool inOpen = false;
	bool inOnsize = false;
	bool mCanResize = true;
	bool mAttached = false;
};

//-----------------------------------------------------------------------------
MyVST3Editor::MyVST3Editor (HostCheckerController* controller, UTF8StringPtr templateName,
                            UTF8StringPtr xmlFile)
: VST3Editor (controller, templateName, xmlFile), hostController (controller)
{
}

//-----------------------------------------------------------------------------
MyVST3Editor::~MyVST3Editor ()
{
	if (checkTimer)
		checkTimer->forget ();
}

//-----------------------------------------------------------------------------
bool PLUGIN_API MyVST3Editor::open (void* parent, const PlatformType& type)
{
	inOpen = true;

	openCount++;

	if (wasAlreadyClosed)
		hostController->addFeatureLog (kLogIdIPlugViewmultipleAttachSupported);

	bool res = VST3Editor::open (parent, type);
	auto hcController = dynamic_cast<HostCheckerController*> (controller);
	if (hcController)
	{
		ViewRect rect;
		if (hcController->getSavedSize (rect))
			onSize (&rect);
	}
	inOpen = false;

	return res;
}

//-----------------------------------------------------------------------------
void PLUGIN_API MyVST3Editor::close ()
{
	wasAlreadyClosed = true;

	openCount--;
	return VST3Editor::close ();
}

//-----------------------------------------------------------------------------
bool MyVST3Editor::beforeSizeChange (const CRect& newSize, const CRect& oldSize)
{
	if (!inOpen && !inOnsize)
	{
		if (!requestResizeGuard && newSize != oldSize)
		{
			onSizeWanted = true;
		}
	}

	bool res = VST3Editor::beforeSizeChange (newSize, oldSize);

	if (!inOpen && !inOnsize && !requestResizeGuard)
	{
		if (!res)
			onSizeWanted = false;
		else
			hostController->addFeatureLog (kLogIdIPlugFrameonResizeViewSupported);

		if (onSizeWanted)
		{
			if (checkTimer == nullptr)
				checkTimer = new CVSTGUITimer (this, 500);
			checkTimer->stop ();
			checkTimer->start ();
		}
	}
	return res;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API MyVST3Editor::onSize (Steinberg::ViewRect* newSize)
{
	inOnsize = true;
	if (!inOpen)
	{
		if (requestResizeGuard)
			hostController->addFeatureLog (kLogIdIPlugViewCalledSync);
		else if (onSizeWanted)
			hostController->addFeatureLog (kLogIdIPlugViewCalledAsync);

		onSizeWanted = false;

		hostController->addFeatureLog (kLogIdIPlugViewonSizeSupported);
	}

	if (openCount == 0)
		hostController->addFeatureLog (kLogIdIPlugViewCalledBeforeOpen);

	auto res = VST3Editor::onSize (newSize);
	inOnsize = false;

	return res;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API MyVST3Editor::canResize ()
{
	hostController->addFeatureLog (kLogIdIPlugViewcanResizeSupported);
	return mCanResize ? kResultTrue : kResultFalse;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API MyVST3Editor::checkSizeConstraint (Steinberg::ViewRect* rect)
{
	hostController->addFeatureLog (kLogIdIPlugViewcheckSizeConstraintSupported);
	return VST3Editor::checkSizeConstraint (rect);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MyVST3Editor::onKeyDown (char16 key, int16 keyMsg, int16 modifiers)
{
	if (!mAttached)
		hostController->addFeatureLog (kLogIdIPlugViewKeyCalledBeforeAttach);

	hostController->addFeatureLog (kLogIdIPlugViewOnKeyDownSupported);

	return VSTGUIEditor::onKeyDown (key, keyMsg, modifiers);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MyVST3Editor::onKeyUp (char16 key, int16 keyMsg, int16 modifiers)
{
	if (!mAttached)
		hostController->addFeatureLog (kLogIdIPlugViewKeyCalledBeforeAttach);

	hostController->addFeatureLog (kLogIdIPlugViewOnKeyUpSupported);

	return VSTGUIEditor::onKeyUp (key, keyMsg, modifiers);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MyVST3Editor::onWheel (float distance)
{
	if (!mAttached)
		hostController->addFeatureLog (kLogIdIPlugViewKeyCalledBeforeAttach);

	hostController->addFeatureLog (kLogIdIPlugViewOnWheelCalled);

	return VSTGUIEditor::onWheel (distance);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MyVST3Editor::onFocus (TBool state)
{
	hostController->addFeatureLog (kLogIdIPlugViewOnFocusCalled);

	return VSTGUIEditor::onFocus (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API MyVST3Editor::setFrame (IPlugFrame* frame)
{
	hostController->addFeatureLog (kLogIdIPlugViewsetFrameSupported);
	return VSTGUIEditor::setFrame (frame);
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API MyVST3Editor::attached (void* parent, FIDString type)
{
	if (mAttached)
		hostController->addFeatureLog (kLogIdIPlugViewattachedWithoutRemoved);

	mAttached = true;
	return VSTGUIEditor::attached (parent, type);
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API MyVST3Editor::removed ()
{
	if (!mAttached)
		hostController->addFeatureLog (kLogIdIPlugViewremovedWithoutAttached);

	mAttached = false;
	return VSTGUIEditor::removed ();
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API MyVST3Editor::setContentScaleFactor (ScaleFactor factor)
{
	hostController->addFeatureLog (kLogIdIPlugViewsetContentScaleFactorSupported);
	return VST3Editor::setContentScaleFactor (factor);
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API MyVST3Editor::findParameter (int32 xPos, int32 yPos,
                                                           ParamID& resultTag)
{
	hostController->addFeatureLog (kLogIdIParameterFinderSupported);
	return VST3Editor::findParameter (xPos, yPos, resultTag);
}

//-----------------------------------------------------------------------------
VSTGUI::CMessageResult MyVST3Editor::notify (CBaseObject* sender, const char* message)
{
	if (sender == checkTimer)
	{
		if (onSizeWanted)
			hostController->addFeatureLog (kLogIdIPlugViewNotCalled);

		checkTimer->forget ();
		checkTimer = nullptr;
		return kMessageNotified;
	}

	return VST3Editor::notify (sender, message);
}

//-----------------------------------------------------------------------------
void MyVST3Editor::valueChanged (CControl* pControl)
{
	if (pControl->getTag () == kBypassTag)
	{
	}
	VST3Editor::valueChanged (pControl);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
HostCheckerController::HostCheckerController ()
{
	mScoreMap.emplace (kLogIdIComponentHandler2Supported, 2);
	mScoreMap.emplace (kLogIdIComponentHandler2SetDirtySupported, 2);
	mScoreMap.emplace (kLogIdIComponentHandler2RequestOpenEditorSupported, 2);
	mScoreMap.emplace (kLogIdIComponentHandler3Supported, 2);
	mScoreMap.emplace (kLogIdIComponentHandlerBusActivationSupported, 1);
	mScoreMap.emplace (kLogIdIProgressSupported, 1);
	mScoreMap.emplace (kLogIdIPlugInterfaceSupportSupported, 2);
	mScoreMap.emplace (kLogIdIPlugFrameonResizeViewSupported, 2);
	mScoreMap.emplace (kLogIdIPrefetchableSupportSupported, 1);
	mScoreMap.emplace (kLogIdAudioPresentationLatencySamplesSupported, 1);
	mScoreMap.emplace (kLogIdIProcessContextRequirementsSupported, 1);

	mScoreMap.emplace (kLogIdProcessContextPlayingSupported, 2);
	mScoreMap.emplace (kLogIdProcessContextRecordingSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextCycleActiveSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextSystemTimeSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextContTimeSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextTimeMusicSupported, 2);
	mScoreMap.emplace (kLogIdProcessContextBarPositionSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextCycleSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextTempoSupported, 2);
	mScoreMap.emplace (kLogIdProcessContextTimeSigSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextChordSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextSmpteSupported, 1);
	mScoreMap.emplace (kLogIdProcessContextClockSupported, 1);
	mScoreMap.emplace (kLogIdCanProcessSampleSize32, 1);
	mScoreMap.emplace (kLogIdCanProcessSampleSize64, 1);
	mScoreMap.emplace (kLogIdGetTailSamples, 1);
	mScoreMap.emplace (kLogIdGetLatencySamples, 2);
	mScoreMap.emplace (kLogIdGetBusArrangements, 1);
	mScoreMap.emplace (kLogIdSetBusArrangements, 1);
	mScoreMap.emplace (kLogIdGetRoutingInfo, 1);
	mScoreMap.emplace (kLogIdActivateAuxBus, 1);
	mScoreMap.emplace (kLogIdParametersFlushSupported, 1);
	mScoreMap.emplace (kLogIdSilentFlagsSupported, 2);
	mScoreMap.emplace (kLogIdSilentFlagsSCSupported, 2);

	mScoreMap.emplace (kLogIdIEditController2Supported, 1);
	mScoreMap.emplace (kLogIdsetKnobModeSupported, 1);
	mScoreMap.emplace (kLogIdopenHelpSupported, 1);
	mScoreMap.emplace (kLogIdopenAboutBoxSupported, 1);
	mScoreMap.emplace (kLogIdIMidiMappingSupported, 1);
	mScoreMap.emplace (kLogIdUnitSupported, 1);
	mScoreMap.emplace (kLogIdGetUnitByBusSupported, 1);
	mScoreMap.emplace (kLogIdChannelContextSupported, 1);
	mScoreMap.emplace (kLogIdINoteExpressionControllerSupported, 1);
	mScoreMap.emplace (kLogIdINoteExpressionPhysicalUIMappingSupported, 1);
	mScoreMap.emplace (kLogIdIKeyswitchControllerSupported, 1);
	mScoreMap.emplace (kLogIdIMidiLearnSupported, 1);
	mScoreMap.emplace (kLogIdIMidiLearn_onLiveMIDIControllerInputSupported, 1);

	mScoreMap.emplace (kLogIdIAttributeListInSetStateSupported, 1);

	mScoreMap.emplace (kLogIdIXmlRepresentationControllerSupported, 1);
	mScoreMap.emplace (kLogIdIAutomationStateSupported, 1);

	mScoreMap.emplace (kLogIdIEditControllerHostEditingSupported, 1);

	mScoreMap.emplace (kLogIdIPlugViewonSizeSupported, 1);
	mScoreMap.emplace (kLogIdIPlugViewcanResizeSupported, 1);
	mScoreMap.emplace (kLogIdIPlugViewcheckSizeConstraintSupported, 1);
	mScoreMap.emplace (kLogIdIPlugViewsetFrameSupported, 1);
	mScoreMap.emplace (kLogIdIPlugViewOnWheelCalled, 1);
	mScoreMap.emplace (kLogIdIPlugViewOnKeyDownSupported, 1);
	mScoreMap.emplace (kLogIdIPlugViewOnKeyUpSupported, 1);
	mScoreMap.emplace (kLogIdIPlugViewOnFocusCalled, 1);
	mScoreMap.emplace (kLogIdIPlugViewsetContentScaleFactorSupported, 1);

	mScoreMap.emplace (kLogIdIParameterFinderSupported, 1);
	mScoreMap.emplace (kLogIdIParameterFunctionNameSupported, 1);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::initialize (FUnknown* context)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::initialize"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdinitializeCalledinWrongThread);
	}

	tresult result = EditControllerEx1::initialize (context);
	if (result == kResultOk)
	{
		// create a unit Latency parameter
		UnitInfo unitInfo;
		unitInfo.id = kUnitId;
		unitInfo.parentUnitId = kRootUnitId; // attached to the root unit
		Steinberg::UString (unitInfo.name, USTRINGSIZE (unitInfo.name)).assign (USTRING ("Setup"));
		unitInfo.programListId = kNoProgramListId;

		auto* unit = new Unit (unitInfo);
		addUnit (unit);

		// add second unit
		unitInfo.id = kUnit2Id;
		unitInfo.parentUnitId = kRootUnitId; // attached to the root unit
		Steinberg::UString (unitInfo.name, USTRINGSIZE (unitInfo.name))
		    .assign (USTRING ("Second Unit"));
		unitInfo.programListId = kNoProgramListId;
		unit = new Unit (unitInfo);
		addUnit (unit);

		parameters.addParameter (STR16 ("Param1"), STR16 (""), 0, 0, ParameterInfo::kCanAutomate,
		                         kParam1Tag);
		parameters.addParameter (STR16 ("Generate Peaks"), STR16 (""), 0, 0,
		                         ParameterInfo::kNoFlags, kGeneratePeaksTag);
		parameters.addParameter (new RangeParameter (
		    STR16 ("Latency"), kLatencyTag, nullptr, 0, HostChecker::kMaxLatency, 0,
		    HostChecker::kMaxLatency, ParameterInfo::kNoFlags, kUnitId, nullptr));
		parameters.addParameter (STR16 ("CanResize"), STR16 (""), 1, 1, ParameterInfo::kNoFlags,
		                         kCanResizeTag);

		parameters.addParameter (new RangeParameter (STR16 ("Scoring"), kScoreTag, nullptr, 0, 100,
		                                             0, 100, ParameterInfo::kIsReadOnly));

		parameters.addParameter (STR16 ("Bypass"), STR16 (""), 1, 0,
		                         ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass,
		                         kBypassTag);

		parameters.addParameter (new RangeParameter (STR16 ("ProgressValue"), kProgressValueTag,
		                                             nullptr, 0, 100, 0, 100,
		                                             ParameterInfo::kIsReadOnly));
		parameters.addParameter (STR16 ("TriggerProgress"), STR16 (""), 1, 0,
		                         ParameterInfo::kNoFlags, kTriggerProgressTag);

		parameters.addParameter (STR16 ("ParamWhichCouldBeHidden"), STR16 (""), 0, 0,
		                         ParameterInfo::kCanAutomate, kParamWhichCouldBeHiddenTag,
		                         kUnit2Id);
		parameters.addParameter (STR16 ("TriggerHidden"), STR16 (""), 1, 0, ParameterInfo::kNoFlags,
		                         kTriggerHiddenTag);

		for (uint32 i = 0; i < HostChecker::kParamWarnCount; i++)
		{
			parameters.addParameter (
			    STR16 ("ProcessWarn"), STR16 (""), HostChecker::kParamWarnStepCount, 0,
			    ParameterInfo::kIsReadOnly | ParameterInfo::kIsHidden, kProcessWarnTag + i);
		}

		mDataSource = VSTGUI::owned (new EventLogDataBrowserSource (this));
	}

	FUnknownPtr<IPlugInterfaceSupport> plugInterfaceSupport (context);
	if (plugInterfaceSupport)
	{
		addFeatureLog (kLogIdIPlugInterfaceSupportSupported);

		if (plugInterfaceSupport->isPlugInterfaceSupported (IAutomationState::iid) == kResultTrue)
			addFeatureLog (kLogIdIAutomationStateSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (IEditControllerHostEditing::iid) ==
		    kResultTrue)
			addFeatureLog (kLogIdIEditControllerHostEditingSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (IMidiMapping::iid) == kResultTrue)
			addFeatureLog (kLogIdIMidiMappingSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (IMidiLearn::iid) == kResultTrue)
			addFeatureLog (kLogIdIMidiLearnSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (ChannelContext::IInfoListener::iid) ==
		    kResultTrue)
			addFeatureLog (kLogIdChannelContextSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (INoteExpressionController::iid) ==
		    kResultTrue)
			addFeatureLog (kLogIdINoteExpressionControllerSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (
		        INoteExpressionPhysicalUIMapping::iid) == kResultTrue)
			addFeatureLog (kLogIdINoteExpressionPhysicalUIMappingSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (IXmlRepresentationController::iid) ==
		    kResultTrue)
			addFeatureLog (kLogIdIXmlRepresentationControllerSupported);
		if (plugInterfaceSupport->isPlugInterfaceSupported (IParameterFunctionName::iid) ==
		    kResultTrue)
			addFeatureLog (kLogIdIParameterFunctionNameSupported);
	}

	return result;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::terminate ()
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::terminate"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdterminateCalledinWrongThread);
	}

	tresult result = EditControllerEx1::terminate ();
	if (result == kResultOk)
	{
		mDataSource = nullptr;
		mDataBrowserMap.clear ();
	}

	if (mProgressTimer)
	{
		mProgressTimer->forget ();
		mProgressTimer = nullptr;
	}
	return result;
}

//-----------------------------------------------------------------------------
float HostCheckerController::updateScoring (int32 iD)
{
	float score = 0;
	float total = 0;

	if (iD >= 0)
		mScoreMap[iD].use = true;

	for (auto& item : mScoreMap)
	{
		auto scoreEntry = item.second;
		total += scoreEntry.factor;
		if (scoreEntry.use)
			score += scoreEntry.factor;
	}
	if (total)
		score = score / total;
	else
		score = 0;

	if (auto val = parameters.getParameter (kScoreTag))
		val->setNormalized (score);

	return score;
}

//-----------------------------------------------------------------------------
void HostCheckerController::onProgressTimer (VSTGUI::CVSTGUITimer* timer)
{
	if (!mInProgress)
	{
		FUnknownPtr<IProgress> progress (componentHandler);
		if (progress)
			progress->start (IProgress::ProgressType::UIBackgroundTask, STR ("Test Progress"),
			                 mProgressID);
		mInProgress = true;
	}
	else
	{
		const float stepInc = 1.0 / 60.0 / 5.0; // ~5sec
		auto newVal = parameters.getParameter (kProgressValueTag)->getNormalized () + stepInc;
		// we have finished
		if (newVal > 1)
			setParamNormalized (kTriggerProgressTag, 0);
		else
		{
			setParamNormalized (kProgressValueTag, newVal);

			FUnknownPtr<IProgress> progress (componentHandler);
			if (progress)
				progress->update (mProgressID, newVal);
		}
	}
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::setComponentState (IBStream* state)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::setComponentState"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdSetComponentStateCalledinWrongThread);
	}

	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	float saved = 0.f;
	if (streamer.readFloat (saved) == false)
		return kResultFalse;
	if (saved != 12345.67f)
	{
		SMTG_ASSERT (false)
	}

	uint32 latency;
	if (streamer.readInt32u (latency) == false)
		return kResultFalse;

	uint32 bypass;
	if (streamer.readInt32u (bypass) == false)
		return kResultFalse;

	setParamNormalized (kBypassTag, bypass > 0 ? 1 : 0);

	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getUnitByBus (MediaType type, BusDirection dir,
                                                        int32 busIndex, int32 channel,
                                                        UnitID& unitId /*out*/)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::getUnitByBus"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdGetUnitByBusCalledinWrongThread);
	}

	if (type == kEvent && dir == kInput)
	{
		if (busIndex == 0 && channel == 0)
		{
			unitId = kRootUnitId;
			return kResultTrue;
		}
	}
	addFeatureLog (kLogIdGetUnitByBusSupported);
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::setComponentHandler (IComponentHandler* handler)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::setComponentHandler"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdSetComponentHandlerCalledinWrongThread);
	}

	tresult res = EditControllerEx1::setComponentHandler (handler);
	if (componentHandler2)
	{
		addFeatureLog (kLogIdIComponentHandler2Supported);

		if (componentHandler2->requestOpenEditor () == kResultTrue)
			addFeatureLog (kLogIdIComponentHandler2RequestOpenEditorSupported);
	}

	FUnknownPtr<IComponentHandler3> handler3 (componentHandler);
	if (handler3)
		addFeatureLog (kLogIdIComponentHandler3Supported);

	FUnknownPtr<IComponentHandlerBusActivation> componentHandlerBusActivationSupport (
	    componentHandler);
	if (componentHandlerBusActivationSupport)
		addFeatureLog (kLogIdIComponentHandlerBusActivationSupported);

	FUnknownPtr<IProgress> progress (componentHandler);
	if (progress)
		addFeatureLog (kLogIdIProgressSupported);

	return res;
}

//-----------------------------------------------------------------------------
int32 PLUGIN_API HostCheckerController::getUnitCount ()
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::getUnitCount"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdGetUnitCountCalledinWrongThread);
	}

	addFeatureLog (kLogIdUnitSupported);
	return EditControllerEx1::getUnitCount ();
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::setParamNormalized (ParamID tag, ParamValue value)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::setParamNormalized"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdSetParamNormalizedCalledinWrongThread);
	}

	//--- ----------------------------------------
	if (tag == kLatencyTag && mLatencyInEdit)
	{
		mWantedLatency = value;
		// return kResultTrue;
	}
	//--- ----------------------------------------
	else if (tag == kParam1Tag)
	{
	}
	//--- ----------------------------------------
	else if (tag == kTriggerHiddenTag)
	{
		auto param = parameters.getParameter (kParamWhichCouldBeHiddenTag);
		auto& info = param->getInfo ();
		if (value > 0.5)
		{
			info.flags |= (ParameterInfo::kIsHidden | ParameterInfo::kIsReadOnly);
			info.flags &= ~ParameterInfo::kCanAutomate;
		}
		else
		{
			info.flags &= (~(ParameterInfo::kIsHidden | ParameterInfo::kIsReadOnly) |
			               ParameterInfo::kCanAutomate);
		}
		auto res = EditControllerEx1::setParamNormalized (tag, value);
		componentHandler->restartComponent (kParamTitlesChanged);
		return res;
	}
	else if (tag == kTriggerProgressTag)
	{
		if (value > 0.5)
		{
			if (mProgressTimer == nullptr)
				mProgressTimer =
				    new CVSTGUITimer ([this] (CVSTGUITimer* timer) { onProgressTimer (timer); },
				                      1000 / 60); // 60 Hz
			mProgressTimer->stop ();
			mProgressTimer->start ();
		}
		else
		{
			if (mProgressTimer)
				mProgressTimer->stop ();
			setParamNormalized (kProgressValueTag, 0);
			mInProgress = false;

			FUnknownPtr<IProgress> progress (componentHandler);
			if (progress)
				progress->finish (mProgressID);
		}
	}
	//--- ----------------------------------------
	else if (tag >= kProcessWarnTag && tag <= kProcessWarnTag + HostChecker::kParamWarnCount)
	{
		bool latencyRestartWanted = false;
		int32 tagOffset = (tag - kProcessWarnTag) * HostChecker::kParamWarnBitCount;
		uint32 idValue = value * HostChecker::kParamWarnStepCount;
		for (uint32 i = 0; i < HostChecker::kParamWarnBitCount; i++)
		{
			if (idValue & (1L << i))
			{
				addFeatureLog (tagOffset + i);
				if (tagOffset + i == kLogIdInformLatencyChanged && componentHandler)
					latencyRestartWanted = true;
			}
		}
		if (latencyRestartWanted)
			componentHandler->restartComponent (kLatencyChanged);
	}

	return EditControllerEx1::setParamNormalized (tag, value);
}

//------------------------------------------------------------------------
tresult HostCheckerController::beginEdit (ParamID tag)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::beginEdit"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdBeginEditCalledinWrongThread);
	}

	if (tag == kLatencyTag)
		mLatencyInEdit = true;

	return EditControllerEx1::beginEdit (tag);
}

//-----------------------------------------------------------------------------
tresult HostCheckerController::endEdit (ParamID tag)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::endEdit"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdEndEditCalledinWrongThread);
	}

	if (tag == kLatencyTag && mLatencyInEdit)
	{
		mLatencyInEdit = false;
		setParamNormalized (tag, mWantedLatency);
	}
	return EditControllerEx1::endEdit (tag);
}

//-----------------------------------------------------------------------------
IPlugView* PLUGIN_API HostCheckerController::createView (FIDString name)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::createView"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdCreateViewCalledinWrongThread);
	}

	FUnknownPtr<IComponentHandlerBusActivation> componentHandlerBusActivationSupport (
	    componentHandler);
	if (componentHandlerBusActivationSupport)
		addFeatureLog (kLogIdIComponentHandlerBusActivationSupported);

	if (ConstString (name) == ViewType::kEditor)
	{
		if (componentHandler2)
		{
			if (componentHandler2->setDirty (true) == kResultTrue)
				addFeatureLog (kLogIdIComponentHandler2SetDirtySupported);
		}

		auto view = new MyVST3Editor (this, "HostCheckerEditor", "hostchecker.uidesc");
		if (sizeFactor != 0)
		{
			ViewRect rect (0, 0, width, height);
			view->setRect (rect);
			view->setZoomFactor (sizeFactor);
		}
		view->canResize (parameters.getParameter (kCanResizeTag)->getNormalized () > 0);

		return view;
	}

	return nullptr;
}

//-----------------------------------------------------------------------------
CView* HostCheckerController::createCustomView (UTF8StringPtr name, const UIAttributes& attributes,
                                                const IUIDescription* description,
                                                VST3Editor* editor)
{
	if (ConstString (name) == "HostCheckerDataBrowser")
	{
		auto item = mDataBrowserMap.find (editor);
		if (item != mDataBrowserMap.end ())
		{
			item->second->remember ();
			return item->second;
		}
		else
		{
			auto dataBrowser = VSTGUI::owned (new CDataBrowser (
			    CRect (0, 0, 100, 100), mDataSource,
			    CDataBrowser::kDrawRowLines | CDataBrowser::kDrawColumnLines |
			        CDataBrowser::kDrawHeader | CDataBrowser::kVerticalScrollbar));

			mDataBrowserMap.emplace (editor, dataBrowser);
			dataBrowser->remember ();
			return dataBrowser;
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
void HostCheckerController::willClose (VSTGUI::VST3Editor* editor)
{
	mDataBrowserMap.erase (editor);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::connect (IConnectionPoint* other)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::connect"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdConnectCalledinWrongThread);
	}

	tresult tResult = ComponentBase::connect (other);
	if (peerConnection)
	{
		for (int32 paramIdx = 0; paramIdx < getParameterCount (); ++paramIdx)
		{
			ParameterInfo paramInfo = {};
			if (getParameterInfo (paramIdx, paramInfo) == kResultOk)
			{
				IPtr<IMessage> newMsg = owned (allocateMessage ());
				if (newMsg)
				{
					newMsg->setMessageID ("Parameter");
					IAttributeList* attr = newMsg->getAttributes ();
					if (attr)
					{
						attr->setInt ("ID", paramInfo.id);
					}

					sendMessage (newMsg);
				}
			}
		}

		FUnknownPtr<IAudioProcessor> proc (other);
		if (proc)
		{
			IPtr<IMessage> newMsg = owned (allocateMessage ());
			if (newMsg)
			{
				newMsg->setMessageID ("LogEvent");
				IAttributeList* attr = newMsg->getAttributes ();
				if (attr)
				{
					attr->setInt ("ID", kLogIdProcessorControllerConnection);
					attr->setInt ("Count", 1);
				}

				notify (newMsg);
			}
		}
	}

	return tResult;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::notify (IMessage* message)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::notify"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdnotifyCalledinWrongThread);
	}

	if (!message)
		return kInvalidArgument;

	if (FIDStringsEqual (message->getMessageID (), "LogEvent"))
	{
		int64 id;
		if (message->getAttributes ()->getInt ("ID", id) != kResultOk)
			return kResultFalse;
		int64 count;
		if (message->getAttributes ()->getInt ("Count", count) != kResultOk)
			return kResultFalse;
		addFeatureLog (id, count, false);
	}

	if (FIDStringsEqual (message->getMessageID (), "Latency"))
	{
		ParamValue value;
		if (message->getAttributes ()->getFloat ("Value", value) == kResultOk)
		{
			componentHandler->restartComponent (kLatencyChanged);
		}
	}

	return ComponentBase::notify (message);
}

//-----------------------------------------------------------------------------
void HostCheckerController::addFeatureLog (int32 iD, int32 count, bool addToLastCount)
{
	updateScoring (iD);

	if (!mDataSource)
		return;

	LogEvent logEvt;
	logEvt.id = iD;
	logEvt.count = count;

	if (mDataSource->updateLog (logEvt, addToLastCount))
	{
		for (auto& item : mDataBrowserMap)
			item.second->invalidateRow (logEvt.id);
	}
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::setKnobMode (KnobMode mode)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::setKnobMode"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdsetKnobModeCalledinWrongThread);
	}

	addFeatureLog (kLogIdsetKnobModeSupported);
	return EditControllerEx1::setKnobMode (mode);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::openHelp (TBool onlyCheck)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::openHelp"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdopenHelpCalledinWrongThread);
	}

	addFeatureLog (kLogIdopenHelpSupported);
	return EditControllerEx1::openHelp (onlyCheck);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::openAboutBox (TBool onlyCheck)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::openAboutBox"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdopenAboutBoxCalledinWrongThread);
	}

	addFeatureLog (kLogIdopenAboutBoxSupported);
	return EditControllerEx1::openAboutBox (onlyCheck);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::setChannelContextInfos (IAttributeList* list)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::setChannelContextInfos"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdsetChannelContextInfosCalledinWrongThread);
	}

	if (!list)
		return kResultFalse;

	// optional we can ask for the Channel Name Length
	int64 length;
	if (list->getInt (ChannelContext::kChannelNameLengthKey, length) == kResultTrue)
	{
	}

	// get the Channel Name where we, as plug-in, are instantiated
	String128 name;
	if (list->getString (ChannelContext::kChannelNameKey, name, sizeof (name)) == kResultTrue)
	{
	}

	// get the Channel UID
	if (list->getString (ChannelContext::kChannelUIDKey, name, sizeof (name)) == kResultTrue)
	{
	}

	// get Channel Index
	int64 index;
	if (list->getInt (ChannelContext::kChannelIndexKey, index) == kResultTrue)
	{
	}

	// get the Channel Color
	int64 color;
	if (list->getInt (ChannelContext::kChannelColorKey, color) == kResultTrue)
	{
		//	ColorSpec channelColor = (ColorSpec)color;
	}

	addFeatureLog (kLogIdChannelContextSupported);

	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getXmlRepresentationStream (
    RepresentationInfo& info /*in*/, IBStream* stream /*out*/)
{
	if (!threadChecker->test (
	        THREAD_CHECK_MSG ("HostCheckerController::getXmlRepresentationStream"),
	        THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetXmlRepresentationStreamCalledinWrongThread);
	}

	addFeatureLog (kLogIdIXmlRepresentationControllerSupported);

	String name (info.name);
	if (name == GENERIC_8_CELLS)
	{
		XmlRepresentationHelper helper (info, "Steinberg Media Technologies", "VST3 Host Checker",
		                                HostCheckerProcessorUID.toTUID (), stream);

		helper.startPage ("Main Page");
		helper.startEndCellOneLayer (LayerType::kKnob, 0);
		helper.startEndCellOneLayer (LayerType::kKnob, 1);
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.endPage ();

		helper.startPage ("Page 2");
		helper.startEndCellOneLayer (LayerType::kSwitch, 2);
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty cell
		helper.startEndCell (); // empty
		helper.endPage ();

		return kResultTrue;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getMidiControllerAssignment (
    int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id)
{
	if (!threadChecker->test (
	        THREAD_CHECK_MSG ("HostCheckerController::getMidiControllerAssignment"),
	        THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetMidiControllerAssignmentCalledinWrongThread);
	}

	addFeatureLog (kLogIdIMidiMappingSupported);

	if (busIndex != 0)
		return kResultFalse;

	switch (midiControllerNumber)
	{
		case ControllerNumbers::kCtrlPan: id = kParam1Tag; return kResultOk;
		case ControllerNumbers::kCtrlExpression: id = kGeneratePeaksTag; return kResultOk;
		case ControllerNumbers::kCtrlEffect1: id = kBypassTag; return kResultOk;
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::onLiveMIDIControllerInput (int32 busIndex, int16 channel,
                                                                     CtrlNumber midiCC)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::onLiveMIDIControllerInput"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdOnLiveMIDIControllerInputCalledinWrongThread);
	}

	addFeatureLog (kLogIdIMidiLearn_onLiveMIDIControllerInputSupported);
	return kResultTrue;
}

//-----------------------------------------------------------------------------
int32 PLUGIN_API HostCheckerController::getNoteExpressionCount (int32 busIndex, int16 channel)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::getNoteExpressionCount"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetNoteExpressionCountCalledinWrongThread);
	}

	addFeatureLog (kLogIdINoteExpressionControllerSupported);
	return 0;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getNoteExpressionInfo (
    int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo& info /*out*/)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::getNoteExpressionInfo"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetNoteExpressionInfoCalledinWrongThread);
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getNoteExpressionStringByValue (
    int32 busIndex, int16 channel, NoteExpressionTypeID id,
    NoteExpressionValue valueNormalized /*in*/, String128 string /*out*/)
{
	if (!threadChecker->test (
	        THREAD_CHECK_MSG ("HostCheckerController::getNoteExpressionStringByValue"),
	        THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetNoteExpressionStringByValueCalledinWrongThread);
	}

	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getNoteExpressionValueByString (
    int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar* string /*in*/,
    NoteExpressionValue& valueNormalized /*out*/)
{
	if (!threadChecker->test (
	        THREAD_CHECK_MSG ("HostCheckerController::getNoteExpressionValueByString"),
	        THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetNoteExpressionValueByStringCalledinWrongThread);
	}

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getPhysicalUIMapping (int32 busIndex, int16 channel,
                                                                PhysicalUIMapList& list)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::getPhysicalUIMapping"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetPhysicalUIMappingCalledinWrongThread);
	}

	addFeatureLog (kLogIdINoteExpressionPhysicalUIMappingSupported);
	return kResultTrue;
}

//--- IKeyswitchController ---------------------------
//------------------------------------------------------------------------
int32 PLUGIN_API HostCheckerController::getKeyswitchCount (int32 busIndex, int16 channel)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::getKeyswitchCount"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetKeyswitchCountCalledinWrongThread);
	}

	addFeatureLog (kLogIdIKeyswitchControllerSupported);
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getKeyswitchInfo (int32 busIndex, int16 channel,
                                                            int32 keySwitchIndex,
                                                            KeyswitchInfo& info /*out*/)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::getKeyswitchInfo"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetKeyswitchInfoCalledinWrongThread);
	}

	addFeatureLog (kLogIdIKeyswitchControllerSupported);
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::setAutomationState (int32 state)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::setAutomationState"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdsetAutomationStateCalledinWrongThread);
	}

	addFeatureLog (kLogIdIAutomationStateSupported);
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::beginEditFromHost (ParamID paramID)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::beginEditFromHost"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdBeginEditFromHostCalledinWrongThread);
	}

	addFeatureLog (kLogIdIEditControllerHostEditingSupported);
	inEditFromHost++;
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::endEditFromHost (ParamID paramID)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::endEditFromHost"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdEndEditFromHostCalledinWrongThread);
	}

	addFeatureLog (kLogIdIEditControllerHostEditingSupported);
	inEditFromHost--;
	if (inEditFromHost < 0)
	{
		addFeatureLog (kLogIdIEditControllerHostEditingMisused);
		inEditFromHost = 0;
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getParameterIDFromFunctionName (UnitID unitID,
                                                                          FIDString functionName,
                                                                          ParamID& paramID)
{
	addFeatureLog (kLogIdIParameterFunctionNameSupported);

	if (FIDStringsEqual (functionName, FunctionNameType::kWetDryMix))
	{
		paramID = kParam1Tag;
	}
	else
		paramID = kNoParamId;

	return (paramID != kNoParamId) ? kResultTrue : kResultFalse;
}

//------------------------------------------------------------------------
void HostCheckerController::extractCurrentInfo (EditorView* editor)
{
	auto rect = editor->getRect ();
	height = rect.getHeight ();
	width = rect.getWidth ();

	auto* vst3editor = dynamic_cast<VST3Editor*> (editor);
	if (vst3editor)
		sizeFactor = vst3editor->getZoomFactor ();
}

//------------------------------------------------------------------------
void HostCheckerController::editorRemoved (EditorView* editor)
{
	extractCurrentInfo (editor);
	editors.erase (std::find (editors.begin (), editors.end (), editor));

	editorsSubCtlerMap.erase (editor);
}

//------------------------------------------------------------------------
void HostCheckerController::editorDestroyed (EditorView* editor)
{
}

//------------------------------------------------------------------------
void HostCheckerController::editorAttached (EditorView* editor)
{
	editors.push_back (editor);
	extractCurrentInfo (editor);
}

//------------------------------------------------------------------------
VSTGUI::IController* HostCheckerController::createSubController (UTF8StringPtr name,
                                                                 const IUIDescription* description,
                                                                 VST3Editor* editor)
{
	if (UTF8StringView (name) == "EditorSizeController")
	{
		auto sizeFunc = [&] (float _sizeFactor) {
			sizeFactor = _sizeFactor;
			for (auto& editor : editors)
			{
				auto* vst3editor = dynamic_cast<VST3Editor*> (editor);
				if (vst3editor)
					vst3editor->setZoomFactor (sizeFactor);
			}
		};
		auto subController = new EditorSizeController (this, sizeFunc, sizeFactor);
		editorsSubCtlerMap.insert ({editor, subController});
		return subController;
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::queryInterface (const TUID iid, void** obj)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::queryInterface"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdQueryInterfaceCalledinWrongThread);
	}

	if (FUnknownPrivate::iidEqual (iid, IMidiMapping::iid))
	{
		addRef ();
		*obj = static_cast<IMidiMapping*> (this);
		addFeatureLog (kLogIdIMidiMappingSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, IEditController2::iid))
	{
		addRef ();
		*obj = static_cast<IEditController2*> (this);
		addFeatureLog (kLogIdIEditController2Supported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, IXmlRepresentationController::iid))
	{
		addRef ();
		*obj = static_cast<IXmlRepresentationController*> (this);
		addFeatureLog (kLogIdIXmlRepresentationControllerSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, ChannelContext::IInfoListener::iid))
	{
		addRef ();
		*obj = static_cast<ChannelContext::IInfoListener*> (this);
		addFeatureLog (kLogIdChannelContextSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, INoteExpressionController::iid))
	{
		addRef ();
		*obj = static_cast<INoteExpressionController*> (this);
		addFeatureLog (kLogIdINoteExpressionControllerSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, INoteExpressionPhysicalUIMapping::iid))
	{
		addRef ();
		*obj = static_cast<INoteExpressionPhysicalUIMapping*> (this);
		addFeatureLog (kLogIdINoteExpressionPhysicalUIMappingSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, IKeyswitchController::iid))
	{
		addRef ();
		*obj = static_cast<IKeyswitchController*> (this);
		addFeatureLog (kLogIdIKeyswitchControllerSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, IMidiLearn::iid))
	{
		addRef ();
		*obj = static_cast<IMidiLearn*> (this);
		addFeatureLog (kLogIdIMidiLearnSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, IAutomationState::iid))
	{
		addRef ();
		*obj = static_cast<IAutomationState*> (this);
		addFeatureLog (kLogIdIAutomationStateSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, IEditControllerHostEditing::iid))
	{
		addRef ();
		*obj = static_cast<IEditControllerHostEditing*> (this);
		addFeatureLog (kLogIdIEditControllerHostEditingSupported);
		return kResultOk;
	}
	else if (FUnknownPrivate::iidEqual (iid, IParameterFunctionName::iid))
	{
		addRef ();
		*obj = static_cast<IParameterFunctionName*> (this);
		addFeatureLog (kLogIdIParameterFunctionNameSupported);
		return kResultOk;
	}

	return EditControllerEx1::queryInterface (iid, obj);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::setState (IBStream* state)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::setState"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdsetStateCalledinWrongThread);
	}

	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	uint32 version = 1;
	if (streamer.readInt32u (version) == false)
		return kResultFalse;

	if (streamer.readInt32u (height) == false)
		return kResultFalse;
	if (streamer.readInt32u (width) == false)
		return kResultFalse;
	if (streamer.readDouble (sizeFactor) == false)
		return kResultFalse;

	for (auto& item : editorsSubCtlerMap)
	{
		item.second->setSizeFactor (sizeFactor);
	}

	// since version 2
	if (version > 1)
	{
		bool canResize = true;
		streamer.readBool (canResize);
		parameters.getParameter (kCanResizeTag)->setNormalized (canResize ? 1 : 0);
	}

	return kResultOk;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API HostCheckerController::getState (IBStream* state)
{
	if (!threadChecker->test (THREAD_CHECK_MSG ("HostCheckerController::getState"),
	                          THREAD_CHECK_EXIT))
	{
		addFeatureLog (kLogIdgetStateCalledinWrongThread);
	}

	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	uint32 version = 2;
	streamer.writeInt32u (version);
	streamer.writeInt32u (height);
	streamer.writeInt32u (width);
	streamer.writeDouble (sizeFactor);

	// since version 2
	bool canResize = parameters.getParameter (kCanResizeTag)->getNormalized () > 0;
	streamer.writeBool (canResize);

	return kResultOk;
}

//-----------------------------------------------------------------------------
}
} // namespaces
