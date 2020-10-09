//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/state/bypassstate.cpp
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

#include "public.sdk/source/vst/testsuite/state/bypasspersistence.h"
#include "public.sdk/source/common/memorystream.h"
#include "public.sdk/source/vst/vstpresetfile.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// VstBypassSaveParamTest
//------------------------------------------------------------------------
BypassPersistenceTest::BypassPersistenceTest (ITestPlugProvider* plugProvider,
                                              ProcessSampleSize sampl)
: AutomationTest (plugProvider, sampl, 100, 1, false)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API BypassPersistenceTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (testResult);

	if (bypassId == -1)
	{
		testResult->addMessage (STR ("This plugin does not have a bypass parameter!!!"));
		return true;
	}
	unprepareProcessing ();

	processData.numSamples = 0;
	processData.numInputs = 0;
	processData.numOutputs = 0;
	processData.inputs = nullptr;
	processData.outputs = nullptr;

	audioEffect->setProcessing (true);

	preProcess (testResult);

	// set bypass on
	// if (paramChanges[0].getParameterId () == bypassId)
	{
		paramChanges[0]->init (bypassId, 1);
		paramChanges[0]->setPoint (0, 0, 1);

		controller->setParamNormalized (bypassId, 1);

		if (controller->getParamNormalized (bypassId) < 1)
		{
			testResult->addErrorMessage (STR ("The bypass parameter was not correctly set!"));
		}
	}

	// flush
	tresult result = audioEffect->process (processData);
	if (result != kResultOk)
	{
		testResult->addErrorMessage (
		    STR ("The component failed to process without audio buffers!"));

		audioEffect->setProcessing (false);
		return false;
	}

	postProcess (testResult);

	audioEffect->setProcessing (false);

	// save State
	FUID uid;
	plugProvider->getComponentUID (uid);

	MemoryStream stream;
	PresetFile::savePreset (&stream, uid, vstPlug, controller, nullptr, 0);

	audioEffect->setProcessing (true);

	preProcess (testResult);

	// set bypass off
	if (paramChanges[0]->getParameterId () == bypassId)
	{
		paramChanges[0]->init (bypassId, 1);
		paramChanges[0]->setPoint (0, 0, 0);

		controller->setParamNormalized (bypassId, 0);

		if (controller->getParamNormalized (bypassId) > 0)
		{
			testResult->addErrorMessage (
			    STR ("The bypass parameter was not correctly set in the controller!"));
		}
	}

	// flush
	result = audioEffect->process (processData);
	if (result != kResultOk)
	{
		testResult->addErrorMessage (
		    STR ("The component failed to process without audio buffers!"));

		audioEffect->setProcessing (false);
		return false;
	}

	postProcess (testResult);

	audioEffect->setProcessing (false);

	// load previous preset
	stream.seek (0, IBStream::kIBSeekSet, nullptr);
	PresetFile::loadPreset (&stream, uid, vstPlug, controller);

	if (controller->getParamNormalized (bypassId) < 1)
	{
		testResult->addErrorMessage (
		    STR ("The bypass parameter is not in sync in the controller!"));
		return false;
	}

	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
