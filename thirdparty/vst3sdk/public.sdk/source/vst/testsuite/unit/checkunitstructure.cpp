//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/unit/checkunitstructure.cpp
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

#include "public.sdk/source/vst/testsuite/unit/checkunitstructure.h"
#include "pluginterfaces/vst/ivstunits.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// UnitStructureTest
//------------------------------------------------------------------------
UnitStructureTest::UnitStructureTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool UnitStructureTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	FUnknownPtr<IUnitInfo> iUnitInfo (controller);
	if (iUnitInfo)
	{
		int32 unitCount = iUnitInfo->getUnitCount ();
		if (unitCount <= 0)
		{
			addMessage (testResult,
			            STR ("No units found, while controller implements IUnitInfo !!!"));
		}

		UnitInfo unitInfo = {};
		UnitInfo tmpInfo = {};
		bool rootFound = false;
		for (int32 unitIndex = 0; unitIndex < unitCount; unitIndex++)
		{
			if (iUnitInfo->getUnitInfo (unitIndex, unitInfo) == kResultOk)
			{
				// check parent Id
				if (unitInfo.parentUnitId != kNoParentUnitId) //-1: connected to root
				{
					bool noParent = true;
					for (int32 i = 0; i < unitCount; ++i)
					{
						if (iUnitInfo->getUnitInfo (i, tmpInfo) == kResultOk)
						{
							if (unitInfo.parentUnitId == tmpInfo.id)
							{
								noParent = false;
								break;
							}
						}
					}
					if (noParent && unitInfo.parentUnitId != kRootUnitId)
					{
						addErrorMessage (
						    testResult, printf ("Unit %03d: Parent does not exist!!", unitInfo.id));
						return false;
					}
				}
				else if (!rootFound)
				{
					// root Unit have always the rootID
					if (unitInfo.id != kRootUnitId)
					{
						// we should have a root unit id
						addErrorMessage (
						    testResult,
						    printf ("Unit %03d: Should be the Root Unit => id should be %03d!!",
						            unitInfo.id, kRootUnitId));
						return false;
					}
					rootFound = true;
				}
				else
				{
					addErrorMessage (
					    testResult,
					    printf ("Unit %03d: Has no parent, but there is a root already.",
					            unitInfo.id));
					return false;
				}
			}
			else
			{
				addErrorMessage (testResult, printf ("Unit %03d: No unit info.", unitInfo.id));
				return false;
			}
		}
		addMessage (testResult, STR ("All units have valid parent IDs."));
	}
	else
	{
		addMessage (testResult, STR ("This component does not support IUnitInfo!"));
	}
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
