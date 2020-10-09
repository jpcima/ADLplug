//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/processing/automation.h
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

#include "public.sdk/source/vst/testsuite/processing/automation.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// AutomationTest
//------------------------------------------------------------------------

//------------------------------------------------------------------------
AutomationTest::AutomationTest (ITestPlugProvider* plugProvider, ProcessSampleSize sampl,
                                int32 everyNSamples, int32 numParams, bool sampleAccuracy)
: ProcessTest (plugProvider, sampl)
, bypassId (kNoParamId)
, countParamChanges (0)
, everyNSamples (everyNSamples)
, numParams (numParams)
, sampleAccuracy (sampleAccuracy)
, onceExecuted (false)
{
}

//------------------------------------------------------------------------
AutomationTest::~AutomationTest ()
{
}

//------------------------------------------------------------------------
tresult PLUGIN_API AutomationTest::queryInterface (const TUID _iid, void** obj)
{
	QUERY_INTERFACE (_iid, obj, FUnknown::iid, IParameterChanges);
	QUERY_INTERFACE (_iid, obj, Vst::IParameterChanges::iid, IParameterChanges);
	return ProcessTest::queryInterface (_iid, obj);
}

//------------------------------------------------------------------------
const char* AutomationTest::getName () const
{
	static std::string text;
	const char* accTxt = "Sample";
	if (!sampleAccuracy)
		accTxt = "Block";
	text = "Accuracy: ";
	text += accTxt;
	if (numParams < 1)
		text += ", All Parameters";
	else
	{
		text += ", ";
		text += std::to_string (numParams);
		text += " Parameters";
	}
	text += ", Change every";
	text += std::to_string (everyNSamples);
	text += " Samples";
	return text.data ();
}

//------------------------------------------------------------------------
bool AutomationTest::setup ()
{
	onceExecuted = false;
	if (!ProcessTest::setup ())
		return false;

	if (!controller)
		return false;
	if ((numParams < 1) || (numParams > controller->getParameterCount ()))
		numParams = controller->getParameterCount ();
	if (audioEffect && (numParams > 0))
	{
		ParameterInfo inf = {};
		for (int32 i = 0; i < numParams; ++i)
		{
			paramChanges.push_back (owned (new ParamChanges));
			tresult r = controller->getParameterInfo (i, inf);
			if (r != kResultTrue)
				return false;
			if ((inf.flags & inf.kCanAutomate) != 0)
				paramChanges[i]->init (inf.id, processSetup.maxSamplesPerBlock);
		}

		for (int32 i = 0; i < controller->getParameterCount (); ++i)
		{
			tresult r = controller->getParameterInfo (i, inf);
			if (r != kResultTrue)
				return false;
			if ((inf.flags & inf.kIsBypass) != 0)
			{
				bypassId = inf.id;
				break;
			}
		}
		return true;
	}
	return numParams == 0;
}

//------------------------------------------------------------------------
bool AutomationTest::run (ITestResult* testResult)
{
	if (!testResult)
		return false;

	printTestHeader (testResult);

	if (numParams == 0)
		addMessage (testResult, STR ("No Parameters present."));
	bool ret = ProcessTest::run (testResult);
	return ret;
}

//------------------------------------------------------------------------
bool AutomationTest::teardown ()
{
	paramChanges.clear ();
	return ProcessTest::teardown ();
}

//------------------------------------------------------------------------
bool AutomationTest::preProcess (ITestResult* testResult)
{
	if (!testResult)
		return false;
	if (paramChanges.empty ())
		return numParams == 0;
	bool check = true;
	for (int32 i = 0; i < numParams; ++i)
	{
		paramChanges[i]->resetPoints ();
		int32 point = 0;
		for (int32 pos = 0; pos < processData.numSamples; pos++)
		{
			bool add = (rand () % everyNSamples) == 0;
			if (!onceExecuted)
			{
				if (pos == 0)
				{
					add = true;
					if (!sampleAccuracy)
						onceExecuted = true;
				}
				else if ((pos == 1) && sampleAccuracy)
				{
					add = true;
					onceExecuted = true;
				}
			}
			if (add)
				check &= paramChanges[i]->setPoint (point++, pos,
				                                    ((float)(rand () % 1000000000)) / 1000000000.0);
		}
		if (check)
			processData.inputParameterChanges = this;
	}
	return check;
}

//------------------------------------------------------------------------
bool AutomationTest::postProcess (ITestResult* testResult)
{
	if (!testResult)
		return false;
	if (paramChanges.empty ())
		return numParams == 0;

	for (int32 i = 0; i < numParams; ++i)
	{
		if ((paramChanges[i]->getPointCount () > 0) &&
		    !paramChanges[i]->havePointsBeenRead (!sampleAccuracy))
		{
			if (sampleAccuracy)
				addMessage (testResult,
				            STR ("   Not all points have been read via IParameterChanges"));
			else
				addMessage (testResult,
				            STR ("   No point at all has been read via IParameterChanges"));

			return true; // should not be a problem
		}
	}
	return true;
}

//------------------------------------------------------------------------
int32 AutomationTest::getParameterCount ()
{
	if (paramChanges.empty ())
		return numParams;
	return static_cast<int32> (paramChanges.size ());
}

//------------------------------------------------------------------------
IParamValueQueue* AutomationTest::getParameterData (int32 index)
{
	if ((index >= 0) && (index < getParameterCount ()))
		return paramChanges[index];
	return nullptr;
}

//------------------------------------------------------------------------
IParamValueQueue* AutomationTest::addParameterData (const ParamID& /*id*/, int32& /*index*/)
{
	return nullptr;
}

//------------------------------------------------------------------------
// FlushParamTest
//------------------------------------------------------------------------
FlushParamTest::FlushParamTest (ITestPlugProvider* plugProvider, ProcessSampleSize sampl)
: AutomationTest (plugProvider, sampl, 100, 1, false)
{
}

//------------------------------------------------------------------------
void FlushParamTest::prepareProcessData ()
{
	processData.numSamples = 0;
	processData.numInputs = 0;
	processData.numOutputs = 0;
	processData.inputs = nullptr;
	processData.outputs = nullptr;
}

//------------------------------------------------------------------------
bool PLUGIN_API FlushParamTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;

	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (testResult);

	unprepareProcessing ();

	prepareProcessData ();

	audioEffect->setProcessing (true);

	preProcess (testResult);

	tresult result = audioEffect->process (processData);
	if (result != kResultOk)
	{
		addErrorMessage (testResult,
		                 STR ("The component failed to process without audio buffers!"));

		audioEffect->setProcessing (false);
		return false;
	}

	postProcess (testResult);

	audioEffect->setProcessing (false);
	return true;
}

//------------------------------------------------------------------------
// FlushParamTest2
//------------------------------------------------------------------------
FlushParamTest2::FlushParamTest2 (ITestPlugProvider* plugProvider, ProcessSampleSize sampl)
: FlushParamTest (plugProvider, sampl)
{
}

//------------------------------------------------------------------------
void FlushParamTest2::prepareProcessData ()
{
	prepareProcessing ();
	processData.numSamples = 0;

	// remember original processData config
	std::swap (numInputs, processData.numInputs);
	std::swap (numOutputs, processData.numOutputs);
	if (processData.inputs)
		std::swap (numChannelsIn, processData.inputs[0].numChannels);
	if (processData.outputs)
		std::swap (numChannelsOut, processData.outputs[0].numChannels);
}

//------------------------------------------------------------------------
bool FlushParamTest2::teardown ()
{
	// restore original processData config for correct deallocation
	std::swap (numInputs, processData.numInputs);
	std::swap (numOutputs, processData.numOutputs);
	if (processData.inputs)
		std::swap (numChannelsIn, processData.inputs[0].numChannels);
	if (processData.outputs)
		std::swap (numChannelsOut, processData.outputs[0].numChannels);

	return FlushParamTest::teardown ();
}

//------------------------------------------------------------------------
// FlushParamTest3
//------------------------------------------------------------------------
FlushParamTest3::FlushParamTest3 (ITestPlugProvider* plugProvider, ProcessSampleSize sampl)
: FlushParamTest (plugProvider, sampl)
{
	paramChanges.clear ();
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
