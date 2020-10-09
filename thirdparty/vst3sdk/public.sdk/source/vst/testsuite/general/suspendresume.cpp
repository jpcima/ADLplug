//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/general/suspendresume.cpp
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

#include "public.sdk/source/vst/testsuite/general/suspendresume.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// SuspendResumeTest
//------------------------------------------------------------------------
SuspendResumeTest::SuspendResumeTest (ITestPlugProvider* plugProvider, ProcessSampleSize sampl)
: TestEnh (plugProvider, sampl)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API SuspendResumeTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	for (int32 i = 0; i < 3; ++i)
	{
		if (audioEffect)
		{
			if (audioEffect->canProcessSampleSize (kSample32) == kResultOk)
				processSetup.symbolicSampleSize = kSample32;
			else if (audioEffect->canProcessSampleSize (kSample64) == kResultOk)
				processSetup.symbolicSampleSize = kSample64;
			else
			{
				addErrorMessage (testResult,
				                 STR ("No appropriate symbolic sample size supported!"));
				return false;
			}

			if (audioEffect->setupProcessing (processSetup) != kResultOk)
			{
				addErrorMessage (testResult, STR ("Process setup failed!"));
				return false;
			}
		}
		tresult result = vstPlug->setActive (true);
		if (result != kResultOk)
			return false;

		result = vstPlug->setActive (false);
		if (result != kResultOk)
			return false;
	}
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
