//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/general/parameterfunctionname.cpp
// Created by  : Steinberg, 04/2020
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

#include "public.sdk/source/vst/testsuite/general/parameterfunctionname.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include "pluginterfaces/vst/ivstparameterfunctionname.h"
#include "pluginterfaces/vst/ivstunits.h"
#include <unordered_map>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// ParameterFunctionNameTest
//------------------------------------------------------------------------
ParameterFunctionNameTest::ParameterFunctionNameTest (ITestPlugProvider* plugProvider)
: TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API ParameterFunctionNameTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	if (!controller)
	{
		addMessage (testResult, STR ("No Edit Controller supplied!"));
		return true;
	}

	FUnknownPtr<IParameterFunctionName> iParameterFunctionName (controller);
	if (!iParameterFunctionName)
	{
		addMessage (testResult, STR ("No IParameterFunctionName support."));
		return true;
	}
	addMessage (testResult, STR ("IParameterFunctionName supported."));

	int32 numParameters = controller->getParameterCount ();
	if (numParameters <= 0)
	{
		addMessage (testResult, STR ("This component does not export any parameters!"));
		return true;
	}

	// used for ID check
	std::unordered_map<int32, int32> paramIds;

	for (int32 i = 0; i < numParameters; ++i)
	{
		ParameterInfo paramInfo = {};

		tresult result = controller->getParameterInfo (i, paramInfo);
		if (result != kResultOk)
		{
			addErrorMessage (testResult, printf ("Parameter %03d: is missing!!!", i));
			return false;
		}

		int32 paramId = paramInfo.id;
		if (paramId < 0)
		{
			addErrorMessage (testResult,
			                 printf ("Parameter %03d (id=%d): Invalid Id!!!", i, paramId));
			return false;
		}

		auto search = paramIds.find (paramId);
		if (search != paramIds.end ())
		{
			addErrorMessage (testResult,
			                 printf ("Parameter %03d (id=%d): ID already used by idx=%03d!!!", i,
			                         paramId, search->second));
			return false;
		}
		else
			paramIds[paramId] = i;
	} // end for each parameter

	FUnknownPtr<IUnitInfo> iUnitInfo2 (controller);
	const CString arrayFunctionName[] = {FunctionNameType::kCompGainReduction,
	                                     FunctionNameType::kCompGainReductionMax,
	                                     FunctionNameType::kCompGainReductionPeakHold,
	                                     FunctionNameType::kCompResetGainReductionMax,
	                                     FunctionNameType::kLowLatencyMode,
	                                     FunctionNameType::kRandomize,
	                                     FunctionNameType::kWetDryMix};
	ParamID paramID;
	for (auto item : arrayFunctionName)
	{
		if (iParameterFunctionName->getParameterIDFromFunctionName (kRootUnitId, item, paramID) ==
		    kResultTrue)
		{
			addMessage (testResult,
			            printf ("FunctionName %s supported => paramID %d", item, paramID));

			auto search = paramIds.find (paramID);
			if (search == paramIds.end ())
			{
				addErrorMessage (
				    testResult,
				    printf ("Parameter (id=%d) for FunctionName %s: not Found!!!", paramID, item));
				return false;
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
