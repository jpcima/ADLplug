//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/unit/scanunits.cpp
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

#include "public.sdk/source/vst/testsuite/unit/scanunits.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include "pluginterfaces/vst/ivstunits.h"
#include <memory>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// UnitInfoTest
//------------------------------------------------------------------------
UnitInfoTest::UnitInfoTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool UnitInfoTest::run (ITestResult* testResult)
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
		else
		{
			addMessage (testResult, printf ("This component has %d unit(s).", unitCount));
		}

		auto unitIds = std::unique_ptr<int32[]> (new int32[unitCount]);

		for (int32 unitIndex = 0; unitIndex < unitCount; unitIndex++)
		{
			UnitInfo unitInfo = {};

			if (iUnitInfo->getUnitInfo (unitIndex, unitInfo) == kResultOk)
			{
				int32 unitId = unitInfo.id;
				unitIds[unitIndex] = unitId;
				if (unitId < 0)
				{
					addErrorMessage (testResult, printf ("Unit %03d: Invalid ID!", unitIndex));
					return false;
				}

				// check if ID is already used by another unit
				for (int32 idIndex = 0; idIndex < unitIndex; idIndex++)
				{
					if (unitIds[idIndex] == unitIds[unitIndex])
					{
						addErrorMessage (testResult,
						                 printf ("Unit %03d: ID already used!!!", unitIndex));
						return false;
					}
				}

				auto unitName = VST3::StringConvert::convert (unitInfo.name);
				if (unitName.empty ())
				{
					addErrorMessage (testResult, printf ("Unit %03d: No name!", unitIndex));
					return false;
				}

				int32 parentUnitId = unitInfo.parentUnitId;
				if (parentUnitId < -1)
				{
					addErrorMessage (testResult,
					                 printf ("Unit %03d: Invalid parent ID!", unitIndex));
					return false;
				}
				else if (parentUnitId == unitId)
				{
					addErrorMessage (
					    testResult,
					    printf ("Unit %03d: Parent ID is equal to Unit ID!", unitIndex));
					return false;
				}

				int32 unitProgramListId = unitInfo.programListId;
				if (unitProgramListId < -1)
				{
					addErrorMessage (testResult,
					                 printf ("Unit %03d: Invalid programlist ID!", unitIndex));
					return false;
				}

				addMessage (
				    testResult,
				    printf ("   Unit%03d (ID = %d): \"%s\" (parent ID = %d, programlist ID = %d)",
				            unitIndex, unitId, unitName.data (), parentUnitId, unitProgramListId));

				// test select Unit
				if (iUnitInfo->selectUnit (unitIndex) == kResultTrue)
				{
					UnitID newSelected = iUnitInfo->getSelectedUnit ();
					if (newSelected != unitIndex)
					{
						addMessage (
						    testResult,
						    printf (
						        "The host has selected Unit ID = %d but getSelectedUnit returns ID = %d!!!",
						        unitIndex, newSelected));
					}
				}
			}
			else
			{
				addMessage (testResult, printf ("Unit%03d: No unit info!", unitIndex));
			}
		}
	}
	else
	{
		addMessage (testResult, STR ("This component has no units."));
	}

	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
