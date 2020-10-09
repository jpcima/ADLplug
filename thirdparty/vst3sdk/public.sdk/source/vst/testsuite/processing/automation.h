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

#pragma once

#include "public.sdk/source/vst/testsuite/processing/process.h"
#include <vector>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

class ParamChanges;
//------------------------------------------------------------------------
/** Test Automation.
\ingroup TestClass */
//------------------------------------------------------------------------
class AutomationTest : public ProcessTest, public IParameterChanges
{
public:
//------------------------------------------------------------------------
	AutomationTest (ITestPlugProvider* plugProvider, ProcessSampleSize sampl, int32 everyNSamples,
	                int32 numParams, bool sampleAccuracy);
	~AutomationTest () override;

	const char* getName () const SMTG_OVERRIDE;
	// ITest
	bool PLUGIN_API setup () SMTG_OVERRIDE;
	bool PLUGIN_API run (ITestResult* testResult) SMTG_OVERRIDE;
	bool PLUGIN_API teardown () SMTG_OVERRIDE;

	// IParameterChanges
	int32 PLUGIN_API getParameterCount () SMTG_OVERRIDE;
	IParamValueQueue* PLUGIN_API getParameterData (int32 index) SMTG_OVERRIDE;
	IParamValueQueue* PLUGIN_API addParameterData (const ParamID& id, int32& index) SMTG_OVERRIDE;

	// FUnknown
	DELEGATE_REFCOUNT (ProcessTest)
	tresult PLUGIN_API queryInterface (const TUID _iid, void** obj) override;

//------------------------------------------------------------------------
protected:
	bool preProcess (ITestResult* testResult) SMTG_OVERRIDE;
	bool postProcess (ITestResult* testResult) SMTG_OVERRIDE;
	ParamID bypassId;

	using ParamChangeVector = std::vector<IPtr<ParamChanges>>;
	ParamChangeVector paramChanges;
	int32 countParamChanges;
	int32 everyNSamples;
	int32 numParams;
	bool sampleAccuracy;
	bool onceExecuted;
};

//------------------------------------------------------------------------
/** Test Parameters Flush (no Buffer).
\ingroup TestClass */
//------------------------------------------------------------------------
class FlushParamTest : public AutomationTest
{
public:
	FlushParamTest (ITestPlugProvider* plugProvider, ProcessSampleSize sampl);
	bool PLUGIN_API run (ITestResult* testResult) SMTG_OVERRIDE;

	DECLARE_VSTTEST ("Parameters Flush (no Buffer)")
protected:
	virtual void prepareProcessData ();
};

//------------------------------------------------------------------------
/** Test Parameters Flush 2 (no Buffer).
\ingroup TestClass */
//------------------------------------------------------------------------
class FlushParamTest2 : public FlushParamTest
{
public:
	FlushParamTest2 (ITestPlugProvider* plugProvider, ProcessSampleSize sampl);

	bool PLUGIN_API teardown () SMTG_OVERRIDE;

	DECLARE_VSTTEST ("Parameters Flush 2 (only numChannel==0)")
protected:
	void prepareProcessData () SMTG_OVERRIDE;
	int32 numInputs {0};
	int32 numOutputs {0};
	int32 numChannelsIn {0};
	int32 numChannelsOut {0};
};

//------------------------------------------------------------------------
/** Test Parameters Flush 3 (no Buffer, no parameter change).
\ingroup TestClass */
//------------------------------------------------------------------------
class FlushParamTest3 : public FlushParamTest
{
public:
	FlushParamTest3 (ITestPlugProvider* plugProvider, ProcessSampleSize sampl);
	DECLARE_VSTTEST ("Parameters Flush 2 (no Buffer, no parameter change)")
protected:
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
