//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/bus/busconsistency.cpp
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

#include "public.sdk/source/vst/testsuite/bus/busconsistency.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// BusConsistencyTest
//------------------------------------------------------------------------
BusConsistencyTest::BusConsistencyTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API BusConsistencyTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	bool failed = false;
	int32 numFalseDescQueries = 0;

	for (MediaType mediaType = kAudio; mediaType < kNumMediaTypes; mediaType++)
	{
		for (BusDirection dir = kInput; dir <= kOutput; dir++)
		{
			int32 numBusses = vstPlug->getBusCount (mediaType, dir);
			if (numBusses > 0)
			{
				auto* busArray = new BusInfo[numBusses];
				if (busArray)
				{
					// get all bus descriptions and save them in an array
					int32 busIndex;
					for (busIndex = 0; busIndex < numBusses; busIndex++)
					{
						memset (&busArray[busIndex], 0, sizeof (BusInfo));
						vstPlug->getBusInfo (mediaType, dir, busIndex, busArray[busIndex]);
					}

					// test by getting descriptions randomly and comparing with saved ones
					int32 randIndex = 0;
					BusInfo info = {};

					for (busIndex = 0;
					     busIndex <= numBusses * TestDefaults::instance ().numIterations;
					     busIndex++)
					{
						randIndex = rand () % (numBusses);

						memset (&info, 0, sizeof (BusInfo));

						/*tresult result =*/vstPlug->getBusInfo (mediaType, dir, randIndex, info);
						if (memcmp ((void*)&busArray[randIndex], (void*)&info, sizeof (BusInfo)) !=
						    TestDefaults::instance ().buffersAreEqual)
						{
							failed |= true;
							numFalseDescQueries++;
						}
					}
					delete[] busArray;
				}
			}
		}
	}

	if (numFalseDescQueries > 0)
	{
		addErrorMessage (
		    testResult,
		    printf (
		        "The component returned %i inconsistent buses! (getBusInfo () returns sometime different info for the same bus!",
		        numFalseDescQueries));
	}

	return failed == false;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
