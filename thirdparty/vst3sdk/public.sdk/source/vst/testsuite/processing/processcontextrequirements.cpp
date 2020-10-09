//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/processing/processcontextrequirements.cpp
// Created by  : Steinberg, 04/2005
// Description : VST Test Suite
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

#include "public.sdk/source/vst/testsuite/processing/processcontextrequirements.h"
#include "public.sdk/source/vst/hosting/module.h"
#include "public.sdk/source/vst/utility/processcontextrequirements.h"
#include "public.sdk/source/vst/utility/versionparser.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace {

//------------------------------------------------------------------------
VST3::Optional<VST3::Version> getPluginSDKVersion (ITestPlugProvider* plugProvider,
                                                   ITestResult* testResult)
{
	FUnknownPtr<ITestPlugProvider2> pp2 (plugProvider);
	if (!pp2)
	{
		addErrorMessage (testResult, STR ("Internal test Error. Expected Interface not there!"));
		return {};
	}
	VST3::Hosting::PluginFactory pluginFactory (pp2->getPluginFactory ());
	if (!pluginFactory.get ())
	{
		addErrorMessage (testResult,
		                 STR ("Internal test Error. Expected PluginFactory not there!"));
		return {};
	}
	FUID fuid;
	if (pp2->getComponentUID (fuid) != kResultTrue)
	{
		addErrorMessage (testResult,
		                 STR ("Internal test Error. Could not query the UID of the plug-in!"));
		return {};
	}
	auto plugClassID = VST3::UID::fromTUID (fuid.toTUID ());
	auto classInfos = pluginFactory.classInfos ();
	auto it = std::find_if (classInfos.begin (), classInfos.end (),
	                        [&] (const auto& element) { return element.ID () == plugClassID; });
	if (it == classInfos.end ())
	{
		addErrorMessage (
		    testResult, STR ("Internal test Error. Could not find the class info of the plug-in!"));
		return {};
	}
	return VST3::Version::parse (it->sdkVersion ());
}

//------------------------------------------------------------------------
} // anonymous

//------------------------------------------------------------------------
// ProcessContextRequirementsTest
//------------------------------------------------------------------------
ProcessContextRequirementsTest::ProcessContextRequirementsTest (ITestPlugProvider* plugProvider)
: TestEnh (plugProvider, kSample32)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API ProcessContextRequirementsTest::setup ()
{
	return TestEnh::setup ();
}

//------------------------------------------------------------------------
bool PLUGIN_API ProcessContextRequirementsTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;

	printTestHeader (testResult);

	// check if plug-in is build with any earlier VST SDK which does not support this interface
	auto sdkVersion = getPluginSDKVersion (plugProvider, testResult);
	if (!sdkVersion)
		return false;
	if (sdkVersion->getMajor () < 3 ||
	    (sdkVersion->getMajor () == 3 && sdkVersion->getMinor () < 7))
	{
		addMessage (testResult,
		            STR ("No ProcessContextRequirements required. Plug-In built with older SDK."));
		return true;
	}

	FUnknownPtr<IProcessContextRequirements> contextRequirements (audioEffect);
	if (contextRequirements)
	{
		ProcessContextRequirements req (contextRequirements->getProcessContextRequirements ());
		addMessage (testResult, STR ("ProcessContextRequirements:"));
		if (req.wantsNone ())
			addMessage (testResult, STR (" - None"));
		else
		{
			if (req.wantsSystemTime ())
				addMessage (testResult, STR (" - SystemTime"));
			if (req.wantsContinousTimeSamples ())
				addMessage (testResult, STR (" - ContinousTimeSamples"));
			if (req.wantsProjectTimeMusic ())
				addMessage (testResult, STR (" - ProjectTimeMusic"));
			if (req.wantsBarPositionMusic ())
				addMessage (testResult, STR (" - BarPosititionMusic"));
			if (req.wantsCycleMusic ())
				addMessage (testResult, STR (" - CycleMusic"));
			if (req.wantsSamplesToNextClock ())
				addMessage (testResult, STR (" - SamplesToNextClock"));
			if (req.wantsTempo ())
				addMessage (testResult, STR (" - Tempo"));
			if (req.wantsTimeSignature ())
				addMessage (testResult, STR (" - TimeSignature"));
			if (req.wantsChord ())
				addMessage (testResult, STR (" - Chord"));
			if (req.wantsFrameRate ())
				addMessage (testResult, STR (" - FrameRate"));
			if (req.wantsTransportState ())
				addMessage (testResult, STR (" - TransportState"));
		}
		return true;
	}

	addMessage (testResult,
	            STR ("Since VST SDK 3.7 you need to implement IProcessContextRequirements!"));
	addErrorMessage (testResult, STR ("Missing mandatory IProcessContextRequirements extension!"));
	return false;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
