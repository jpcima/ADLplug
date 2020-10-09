//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/logevents.h
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

#include "pluginterfaces/base/ftypes.h"
#include "base/source/fstring.h"
#include <map>

//------------------------------------------------------------------------
struct LogEvent
{
	LogEvent () : id (-1), count (0), fromProcessor (false) {}

	LogEvent (const LogEvent& other)
	: id (other.id), count (other.count), fromProcessor (other.fromProcessor)
	{
	}

	Steinberg::int64 id;
	Steinberg::int64 count;
	bool fromProcessor;
};

//------------------------------------------------------------------------
// Categories
#define SETUP_CONTEXT "SetupContext"
#define STATE "State"
#define AUDIO_BUFFER "AudioBuffer"
#define EVENT_LIST "EventList"
#define PARAM_CHANGE "ParameterChanges"
#define PROCESS_DATA "ProcessData"
#define PROCESS_CONTEXT "ProcessContext"
#define THREAD_CONTEXT "ThreadContext"
#define FEATURE_SUPPORT "FeatureSupport"
#define HOST_FEATURE_SUPPORT "HostFeatureSupport"
#define FEATURE_PROCESSOR_SUPPORT "FeatureProcessSupport"

#define PROCESS true
#define CONTROL false

// Severity
#define LOG_ERR "Error"
#define LOG_WARN "Warn"
#define LOG_INFO "Info"


#define LOG_EVENT_LIST(LOG_DEF) \
	LOG_DEF(kLogIdProcessorControllerConnection,CONTROL,	LOG_WARN, SETUP_CONTEXT, "Processor and controller are directly connected (direct pointers no wrapper)."), \
	LOG_DEF(kLogIdInvalidActivateAuxBus,		PROCESS,	LOG_ERR, SETUP_CONTEXT, "Unknown bus to activate!"),\
	LOG_DEF(kLogIdInvalidStateInitializedMissing,PROCESS,	LOG_ERR, STATE, "Missing State: Uninitialized => Initialized."), \
	LOG_DEF(kLogIdInvalidStateSetupMissing,		PROCESS,	LOG_ERR, STATE, "Missing State: Initialized => Setup Done."), \
	LOG_DEF(kLogIdInvalidStateActivatedMissing,	PROCESS,	LOG_ERR, STATE, "Missing State: Setup Done => Activated."), \
	LOG_DEF(kLogIdInvalidStateProcessingMissing,PROCESS,	LOG_ERR, STATE, "Missing State: Activated => Processing."), \
	LOG_DEF(kLogIdInvalidStateSetActiveWrong,	PROCESS,	LOG_ERR, STATE, "Wrong Call Order: setActive () called in a Processing State."), \
	LOG_DEF(kLogIdInvalidStateSetProcessingWrong,PROCESS,	LOG_ERR, STATE, "Wrong Call Order: setProcessing () called in not Activated State."), \
	LOG_DEF(kLogIdsetActiveFalseRedundant,		PROCESS,	LOG_WARN, STATE, "Redondant Call: setActive (false)!"), \
	LOG_DEF(kLogIdsetActiveTrueRedundant,		PROCESS,	LOG_WARN, STATE, "Redondant Call: setActive (true)!"), \
	LOG_DEF(kLogIdsetProcessingFalseRedundant,	PROCESS,	LOG_WARN, STATE, "Redondant Call: setProcessing (false)!"), \
	LOG_DEF(kLogIdsetProcessingTrueRedundant,	PROCESS,	LOG_WARN, STATE, "Redondant Call: setProcessing (true)!"), \
	LOG_DEF(kLogIdInvalidSymbolicSampleSize,	PROCESS,	LOG_ERR, PROCESS_DATA, "Symbolic sample size does not match the one in ProcessSetup"), \
	LOG_DEF(kLogIdInvalidProcessMode,			PROCESS,	LOG_ERR, PROCESS_DATA, "Process mode does not match the one in ProcessSetup."),\
	LOG_DEF(kLogIdInvalidBlockSize,				PROCESS,	LOG_ERR, PROCESS_DATA, "Block size is either < 1 or >= max block size."),\
	LOG_DEF(kLogIdProcessContextPointerNull,	PROCESS,	LOG_WARN, PROCESS_DATA, "Pointer to ProcessContext struct is null."),\
	LOG_DEF(kLogIdInvalidProcessContextSampleRate,	PROCESS, LOG_ERR, PROCESS_CONTEXT, "The sampleRate does not match the one in ProcessSetup."),\
	LOG_DEF(kLogIdNullPointerToChannelBuf,		PROCESS,	LOG_ERR, AUDIO_BUFFER, "A pointer to a channel buffer is null although the index is valid."),\
	LOG_DEF(kLogIdNullPointerToAuxChannelBuf,	PROCESS,	LOG_ERR, AUDIO_BUFFER, "A pointer to a SideChain channel buffer is null although the index is valid."),\
	LOG_DEF(kLogIdNullPointerToAudioBusBuffer,	PROCESS,	LOG_ERR, AUDIO_BUFFER, "A pointer to an audio bus buffer is null although the index is valid."),\
	LOG_DEF(kLogIdAudioBufNotMatchComponentBusCount,PROCESS, LOG_ERR, AUDIO_BUFFER, "Number of Audio Buffers does not match the number of busses defined by IComponent."),\
	LOG_DEF(kLogIdInvalidAudioBufNumOfChannels,	PROCESS,	LOG_ERR, AUDIO_BUFFER, "An audio bus number of channels is different from the one specified by IComponent."),\
	LOG_DEF(kLogIdUnknownEventType,				PROCESS,	LOG_ERR, EVENT_LIST, "Event has a type which is not specified."),\
	LOG_DEF(kLogIdInvalidEventVelocityValue,	PROCESS,	LOG_ERR, EVENT_LIST, "Event velocity is either < 0.0 or > 1.0."),\
	LOG_DEF(kLogIdInvalidEventPitchValue,		PROCESS,	LOG_ERR, EVENT_LIST, "Event pitch is either < 0 or > 127."),\
	LOG_DEF(kLogIdInvalidEventSampleOffset,		PROCESS,	LOG_ERR, EVENT_LIST, "Event sample offset either < 0 or >= max block size."),\
	LOG_DEF(kLogIdInvalidEventBusIndex,			PROCESS,	LOG_ERR, EVENT_LIST, "Event has a bus index which is different from the one specified by IComponent."),\
	LOG_DEF(kLogIdInvalidNoteOnChannelIndex,	PROCESS,	LOG_ERR, EVENT_LIST, "Note On event has a channel index which was not specified by IComponent."),\
	LOG_DEF(kLogIdInvalidNoteOffChannelIndex,	PROCESS,	LOG_ERR, EVENT_LIST, "Note Off event has a channel index which was not specified by IComponent."),\
	LOG_DEF(kLogIdInvalidPolyPressChannelIndex,	PROCESS,	LOG_ERR, EVENT_LIST, "Poly pressure event has a channel index which was not specified by IComponent."),\
	LOG_DEF(kLogIdNumInputEventExceedsLimit,	PROCESS,	LOG_WARN, EVENT_LIST, "List contains more than 2048 events."),\
	LOG_DEF(kLogIdCouldNotGetAnInputEvent,		PROCESS,	LOG_WARN, EVENT_LIST, "Getting an event returned an error code."),\
	LOG_DEF(kLogIdEventsAreNotSortedBySampleOffset,	PROCESS, LOG_WARN, EVENT_LIST, "Events are not sorted by sample offset."),\
	LOG_DEF(kLogIdEventsAreNotSortedByPpqPosition,	PROCESS, LOG_WARN, EVENT_LIST, "Events are not sorted by PPQ position."),\
	LOG_DEF(kLogIdNoteOnWithPitchAlreadyTriggered,	PROCESS, LOG_INFO, EVENT_LIST, "An event occurred with a pitch currently already triggered."),\
	LOG_DEF(kLogIdNoteOnWithIdAlreadyTriggered,		PROCESS, LOG_WARN, EVENT_LIST, "An event occurred with an ID currently already triggered."),\
	LOG_DEF(kLogIdNoteOffWithIdNeverTriggered,		PROCESS, LOG_WARN, EVENT_LIST, "A Note Off event with no matching note On (ID)"),\
	LOG_DEF(kLogIdNoteOffWithPitchNeverTriggered,	PROCESS, LOG_WARN, EVENT_LIST, "A Note Off event with no matching note On (pitch)."),\
	LOG_DEF(kLogIdNoteExpressValNotNormalized,		PROCESS, LOG_ERR, EVENT_LIST,  "A note expression event value is either < 0.0 or > 1.0."),\
	LOG_DEF(kLogIdInvalidParamValue,				PROCESS, LOG_ERR, PARAM_CHANGE, "Parameter value is < 0.0 or > 1.0"),\
	LOG_DEF(kLogIdInvalidParameterCount,			PROCESS, LOG_ERR, PARAM_CHANGE, "The number of changes is bigger than the number of parameters specified by IEditController."),\
	LOG_DEF(kLogIdInvalidParameterID,				PROCESS, LOG_ERR, PARAM_CHANGE, "A parameter change queue has a parameter ID which was not specified by IEditController."),\
	LOG_DEF(kLogIdParameterChangesPointerIsNull,	PROCESS, LOG_WARN, PARAM_CHANGE, "Pointer to parameter changes interface is null."),\
	LOG_DEF(kLogIdParameterQueueIsNullForValidIndex, PROCESS, LOG_ERR, PARAM_CHANGE, "Pointer to parameter value queue interface is null, although index is valid."),\
	LOG_DEF(kLogIdParametersAreNotSortedBySampleOffset, PROCESS, LOG_ERR, PARAM_CHANGE, "Parameter changes (for a ID) are not sorted by sample offset."),\
	LOG_DEF(kLogIdParametersHaveSameSampleOffset,   PROCESS, LOG_WARN, PARAM_CHANGE, "Parameter changes (for a ID) have more than one time the same sample offset."),\
	\
	LOG_DEF(kLogIdinitializeCalledinWrongThread,		CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::initialize is called in wrong Thread!"),\
	LOG_DEF(kLogIdterminateCalledinWrongThread,			CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::terminate is called in wrong Thread!"),\
	LOG_DEF(kLogIdQueryInterfaceCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::queryInterface is called in wrong Thread!"),\
	LOG_DEF(kLogIdSetComponentHandlerCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::setComponentHandler is called in wrong Thread!"),\
	LOG_DEF(kLogIdSetComponentStateCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::setComponentState is called in wrong Thread!"),\
	LOG_DEF(kLogIdConnectCalledinWrongThread,			CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::connect is called in wrong Thread!"),\
	LOG_DEF(kLogIdsetStateCalledinWrongThread,			CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::setState is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetStateCalledinWrongThread,			CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getState is called in wrong Thread!"),\
	LOG_DEF(kLogIdnotifyCalledinWrongThread,			CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::notify is called in wrong Thread!"),\
	LOG_DEF(kLogIdGetUnitByBusCalledinWrongThread,		CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getUnitByBus is called in wrong Thread!"),\
	LOG_DEF(kLogIdGetUnitCountCalledinWrongThread,		CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getUnitCount is called in wrong Thread!"),\
	LOG_DEF(kLogIdSetParamNormalizedCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::setParamNormalized is called in wrong Thread!"),\
	LOG_DEF(kLogIdBeginEditCalledinWrongThread,			CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::beginEdit is called in wrong Thread!"),\
	LOG_DEF(kLogIdEndEditCalledinWrongThread,			CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::endEdit is called in wrong Thread!"),\
	LOG_DEF(kLogIdBeginEditFromHostCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::beginEditFromHost is called in wrong Thread!"),\
	LOG_DEF(kLogIdEndEditFromHostCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::endEditFromHost is called in wrong Thread!"),\
	LOG_DEF(kLogIdCreateViewCalledinWrongThread,		CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::createView is called in wrong Thread!"),\
	LOG_DEF(kLogIdOnLiveMIDIControllerInputCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::onLiveMIDIControllerInput is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetNoteExpressionCountCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getNoteExpressionCount is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetNoteExpressionInfoCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getNoteExpressionInfo is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetNoteExpressionValueByStringCalledinWrongThread, CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getNoteExpressionValueByString is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetNoteExpressionStringByValueCalledinWrongThread, CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getNoteExpressionStringByValue is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetPhysicalUIMappingCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getPhysicalUIMapping is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetKeyswitchCountCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getKeyswitchCount is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetKeyswitchInfoCalledinWrongThread,	CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getKeyswitchInfo is called in wrong Thread!"),\
	LOG_DEF(kLogIdsetAutomationStateCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::setAutomationState is called in wrong Thread!"),\
	LOG_DEF(kLogIdsetKnobModeCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::setKnob is called in wrong Thread!"),\
	LOG_DEF(kLogIdopenHelpCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::openHelp is called in wrong Thread!"),\
	LOG_DEF(kLogIdopenAboutBoxCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::openAbout is called in wrong Thread!"),\
	LOG_DEF(kLogIdsetChannelContextInfosCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::setChannelContextInfos is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetMidiControllerAssignmentCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getMidiControllerAssignment is called in wrong Thread!"),\
	LOG_DEF(kLogIdgetXmlRepresentationStreamCalledinWrongThread,CONTROL, LOG_ERR, THREAD_CONTEXT, "IEditController::getXmlRepresentationStream is called in wrong Thread!"),\
	LOG_DEF(kLogIdSetActiveCalledinWrongThread,			PROCESS, LOG_ERR, THREAD_CONTEXT, "IComponent::setActive is called in wrong Thread!"),\
	LOG_DEF(kLogIdProcessorSetStateCalledinWrongThread,	PROCESS, LOG_ERR, THREAD_CONTEXT, "IComponent::setState is called in wrong Thread!"),\
	LOG_DEF(kLogIdProcessorGetStateCalledinWrongThread,	PROCESS, LOG_ERR, THREAD_CONTEXT, "IComponent::getState is called in wrong Thread!"),\
	LOG_DEF(kLogIdactivateBusCalledinWrongThread,		PROCESS, LOG_ERR, THREAD_CONTEXT, "IComponent::activateBus is called in wrong Thread!"),\
	\
	LOG_DEF(kLogIdSetActiveCalledSupported,          PROCESS, LOG_INFO, HOST_FEATURE_SUPPORT, "IComponent::setActive (true) called"), \
	LOG_DEF(kLogIdIAttributeListInSetStateSupported, PROCESS, LOG_INFO, HOST_FEATURE_SUPPORT, "IAttributeList in setState supported!"), \
	LOG_DEF(kLogIdIComponentHandler2Supported,      CONTROL, LOG_INFO, HOST_FEATURE_SUPPORT, "IComponentHandler2 supported!"), \
	LOG_DEF(kLogIdIComponentHandler2SetDirtySupported, CONTROL, LOG_INFO, HOST_FEATURE_SUPPORT, "IComponentHandler2::setDirty supported!"), \
	LOG_DEF(kLogIdIComponentHandler2RequestOpenEditorSupported, CONTROL, LOG_INFO, HOST_FEATURE_SUPPORT, "IComponentHandler2::requestOpenEditor supported!"), \
	LOG_DEF(kLogIdIComponentHandler3Supported,		CONTROL, LOG_INFO, HOST_FEATURE_SUPPORT, "IComponentHandler3 (contextMenu) supported!"), \
	LOG_DEF (kLogIdIComponentHandlerBusActivationSupported,	CONTROL, LOG_INFO, HOST_FEATURE_SUPPORT, "IComponentHandlerBusActivation supported!"), \
	LOG_DEF (kLogIdIProgressSupported,	            CONTROL, LOG_INFO, HOST_FEATURE_SUPPORT, "IProgress supported!"), \
	LOG_DEF(kLogIdIPlugInterfaceSupportSupported,   CONTROL, LOG_INFO, HOST_FEATURE_SUPPORT, "IPlugInterfaceSupport supported!"), \
	LOG_DEF(kLogIdIPlugFrameonResizeViewSupported,	CONTROL, LOG_INFO, HOST_FEATURE_SUPPORT, "IPlugFrame::resizeView supported!"), \
	LOG_DEF(kLogIdIPrefetchableSupportSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IPrefetchableSupport supported!"),\
	LOG_DEF(kLogIdAudioPresentationLatencySamplesSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioPresentationLatency supported!"), \
	LOG_DEF(kLogIdIProcessContextRequirementsSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IProcessContextRequirements supported!"), \
	\
	LOG_DEF(kLogIdProcessContextPlayingSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kPlaying supported!"), \
	LOG_DEF(kLogIdProcessContextRecordingSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kRecording supported!"), \
	LOG_DEF(kLogIdProcessContextCycleActiveSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kCycleActive supported!"), \
	LOG_DEF(kLogIdProcessContextSystemTimeSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kSystemTimeValid supported!"), \
	LOG_DEF(kLogIdProcessContextContTimeSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kContTimeValid supported!"), \
	LOG_DEF(kLogIdProcessContextTimeMusicSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kProjectTimeMusicValid supported!"), \
	LOG_DEF(kLogIdProcessContextBarPositionSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kBarPositionValid supported!"), \
	LOG_DEF(kLogIdProcessContextCycleSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kCycleValid supported!"), \
	LOG_DEF(kLogIdProcessContextTempoSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kTempoValid supported!"), \
	LOG_DEF(kLogIdProcessContextTimeSigSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kTimeSigValid supported!"), \
	LOG_DEF(kLogIdProcessContextChordSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kChordValid supported!"), \
	LOG_DEF(kLogIdProcessContextSmpteSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kSmpteValid supported!"), \
	LOG_DEF(kLogIdProcessContextClockSupported, PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "ProcessContext::kClockValid supported!"), \
	\
	LOG_DEF(kLogIdCanProcessSampleSize32,		PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::canProcessSampleSize for kSample32 supported!"), \
	LOG_DEF(kLogIdCanProcessSampleSize64,		PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::canProcessSampleSize for kSample64 supported!"), \
	LOG_DEF(kLogIdGetTailSamples,				PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::getTailSamples supported!"), \
	LOG_DEF(kLogIdGetLatencySamples,			PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::getLatencySamples supported!"), \
	LOG_DEF(kLogIdGetBusArrangements,			PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::getBusArrangements supported!"), \
	LOG_DEF(kLogIdSetBusArrangements,			PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::setBusArrangements supported!"), \
	LOG_DEF(kLogIdGetRoutingInfo,				PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IComponent::getRoutingInfo supported!"), \
	LOG_DEF(kLogIdActivateAuxBus,				PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IComponent::activateBus for SideChain supported!"), \
	LOG_DEF(kLogIdParametersFlushSupported,		PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::process called for flush parameter supported!"), \
	LOG_DEF(kLogIdSilentFlagsSupported,			PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::process: silent flags for Main Input supported!"), \
	LOG_DEF(kLogIdSilentFlagsSCSupported,		PROCESS, LOG_INFO, FEATURE_PROCESSOR_SUPPORT, "IAudioProcessor::process: silent flags for SideChain-In supported!"), \
	\
	LOG_DEF(kLogIdIEditController2Supported,	CONTROL, LOG_INFO, FEATURE_SUPPORT, "IEditController2 supported!"), \
	LOG_DEF(kLogIdsetKnobModeSupported,			CONTROL, LOG_INFO, FEATURE_SUPPORT, "IEditController2::setKnobMode supported!"), \
	LOG_DEF(kLogIdopenHelpSupported,			CONTROL, LOG_INFO, FEATURE_SUPPORT, "IEditController2::openHelp supported!"), \
	LOG_DEF(kLogIdopenAboutBoxSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT, "IEditController2::openAboutBox supported!"), \
	\
	LOG_DEF(kLogIdIMidiMappingSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT, "IMidiMapping supported!"), \
	LOG_DEF(kLogIdUnitSupported,				CONTROL, LOG_INFO, FEATURE_SUPPORT, "Unit supported!"), \
	LOG_DEF(kLogIdGetUnitByBusSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT, "IUnitInfo::getUnitByBus supported!"), \
	LOG_DEF(kLogIdChannelContextSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT, "ChannelContext::IInfoListener supported!"), \
	LOG_DEF(kLogIdINoteExpressionControllerSupported, CONTROL, LOG_INFO, FEATURE_SUPPORT, "INoteExpressionController supported!"), \
	LOG_DEF(kLogIdINoteExpressionPhysicalUIMappingSupported, CONTROL, LOG_INFO, FEATURE_SUPPORT, "INoteExpressionPhysicalUIMapping supported!"), \
	LOG_DEF(kLogIdIKeyswitchControllerSupported, CONTROL, LOG_INFO, FEATURE_SUPPORT, "IKeyswitchController supported!"), \
	\
	LOG_DEF(kLogIdIMidiLearnSupported,			CONTROL, LOG_INFO, FEATURE_SUPPORT, "IMidiLearn supported!"), \
	LOG_DEF(kLogIdIMidiLearn_onLiveMIDIControllerInputSupported, CONTROL, LOG_INFO, FEATURE_SUPPORT, "IMidiLearn::onLiveMIDIControllerInput supported!"), \
	\
	LOG_DEF(kLogIdIXmlRepresentationControllerSupported, CONTROL,  LOG_INFO, FEATURE_SUPPORT, "XmlRepresentation supported!"),\
	LOG_DEF (kLogIdIAutomationStateSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT, "IAutomationState supported!"), \
	LOG_DEF (kLogIdIEditControllerHostEditingSupported,	CONTROL, LOG_INFO, FEATURE_SUPPORT, "IEditControllerHostEditing supported!"), \
	LOG_DEF(kLogIdIEditControllerHostEditingMisused,	CONTROL, LOG_ERR, FEATURE_SUPPORT,	"IEditControllerHostEditing::beginEditFromHost-endEditFromHost not correctly used!"), \
	\
	LOG_DEF(kLogIdIPlugViewonSizeSupported,			CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::onSize supported!"), \
	LOG_DEF(kLogIdIPlugViewcanResizeSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::canResize supported!"), \
	LOG_DEF(kLogIdIPlugViewcheckSizeConstraintSupported, CONTROL, LOG_INFO, FEATURE_SUPPORT, "IPlugView::checkSizeConstraint supported!"), \
	LOG_DEF(kLogIdIPlugViewsetFrameSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::setFrame supported!"), \
	LOG_DEF(kLogIdIPlugViewOnWheelCalled,			CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::onWheel supported!"), \
	LOG_DEF(kLogIdIPlugViewOnKeyDownSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::onKeyDown supported!"), \
	LOG_DEF(kLogIdIPlugViewOnKeyUpSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::onKeyUp supported!"), \
	LOG_DEF(kLogIdIPlugViewOnFocusCalled,			CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::onFocus supported!"), \
	LOG_DEF(kLogIdIPlugViewsetContentScaleFactorSupported, CONTROL, LOG_INFO, FEATURE_SUPPORT, "IPlugViewContentScaleSupport::setContentScaleFactor supported!"), \
	\
	LOG_DEF(kLogIdIPlugViewmultipleAttachSupported, CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::attached-removed called multiple time."), \
	LOG_DEF(kLogIdIPlugViewCalledSync,				CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::onSize is called sync during a resizeView."), \
	LOG_DEF(kLogIdIPlugViewCalledBeforeOpen,		CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IPlugView::onSize is called before attached."), \
	\
	LOG_DEF(kLogIdIPlugViewKeyCalledBeforeAttach,	CONTROL, LOG_ERR, FEATURE_SUPPORT,	"IPlugView::onKeyUp or onKeyDown or onWheel is called before attached!"), \
	LOG_DEF(kLogIdIPlugViewNotCalled,				CONTROL, LOG_ERR, FEATURE_SUPPORT,	"IPlugView::onSize not called after a resizeView!"), \
	LOG_DEF(kLogIdIPlugViewCalledAsync,				CONTROL, LOG_ERR, FEATURE_SUPPORT,	"IPlugView::onSize is called async after a resizeView. Should be Sync!"), \
	LOG_DEF(kLogIdIPlugViewattachedWithoutRemoved,	CONTROL, LOG_ERR, FEATURE_SUPPORT,	"IPlugView::attached is called without removed first!"), \
	LOG_DEF(kLogIdIPlugViewremovedWithoutAttached,	CONTROL, LOG_ERR, FEATURE_SUPPORT,	"IPlugView::removed is called without attached first!"), \
	\
	LOG_DEF(kLogIdIParameterFinderSupported,		CONTROL, LOG_INFO, FEATURE_SUPPORT,	"IParameterFinder supported!"), \
	LOG_DEF(kLogIdIParameterFunctionNameSupported,	CONTROL, LOG_INFO, FEATURE_SUPPORT, "IParameterFunctionName supported!"), \
	\
	LOG_DEF(kLogIdInformLatencyChanged,				PROCESS, LOG_INFO, PARAM_CHANGE,	"InformLatencyChanged called from processor.")

#define LOG_ID(a, b, c, d, e) a
#define LOG_SEVER(a, b, c, d, e) c
#define LOG_DESC(a, b, c, d, e) ("[" d "] " e) // "[category] description"
#define LOG_CONTEXT(a, b, c, d, e) b

// enum of all IDs
enum eLogIds
{
	LOG_EVENT_LIST (LOG_ID),

	kNumLogEvents,
};

// array of bool process : 'true' or controller : 'false'
static const bool logEventContext[] = {LOG_EVENT_LIST (LOG_CONTEXT)};

// array of log descriptions
static const char* logEventDescriptions[] = {LOG_EVENT_LIST (LOG_DESC)};

// array of string 'error' or 'warning'
static const char* logEventSeverity[] = {LOG_EVENT_LIST (LOG_SEVER)};
