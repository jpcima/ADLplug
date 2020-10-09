//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/bus/businvalidindex.cpp
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

#include "public.sdk/source/vst/testsuite/bus/businvalidindex.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// BusInvalidIndexTest
//------------------------------------------------------------------------
BusInvalidIndexTest::BusInvalidIndexTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API BusInvalidIndexTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	bool failed = false;
	int32 numInvalidDesc = 0;

	for (MediaType mediaType = kAudio; mediaType < kNumMediaTypes; mediaType++)
	{
		int32 numBusses =
		    vstPlug->getBusCount (mediaType, kInput) + vstPlug->getBusCount (mediaType, kOutput);
		for (BusDirection dir = kInput; dir <= kOutput; dir++)
		{
			BusInfo descBefore = {};
			BusInfo descAfter = {};

			int32 randIndex = 0;

			// todo: rand with negative numbers
			for (int32 i = 0; i <= numBusses * TestDefaults::instance ().numIterations; ++i)
			{
				randIndex = rand ();
				if (0 > randIndex || randIndex > numBusses)
				{
					/*tresult result =*/vstPlug->getBusInfo (mediaType, dir, randIndex, descAfter);

					if (memcmp ((void*)&descBefore, (void*)&descAfter, sizeof (BusInfo)) != 0)
					{
						failed |= true;
						numInvalidDesc++;
					}
				}
			}
		}
	}

	if (numInvalidDesc > 0)
	{
		addErrorMessage (testResult,
		                 printf ("The component returned %i buses queried with an invalid index!",
		                         numInvalidDesc));
	}

	return failed == false;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
