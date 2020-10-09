//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : testsuite.h
// Created by  : Steinberg, 05/2019
// Description : VST 3 Plug-in Validator test suite class
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
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once

#include "base/source/fobject.h"
#include "pluginterfaces/test/itest.h"
#include <vector>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// TestSuite
//------------------------------------------------------------------------
class TestSuite : public ITestSuite, public FObject
{
public:
	TestSuite (FIDString _name) : name (_name) {}

	tresult PLUGIN_API addTest (FIDString _name, ITest* test) SMTG_OVERRIDE
	{
		tests.push_back (IPtr<Test> (NEW Test (_name, test), false));
		return kResultTrue;
	}

	tresult PLUGIN_API addTestSuite (FIDString _name, ITestSuite* testSuite) SMTG_OVERRIDE
	{
		testSuites.push_back (std::make_pair (_name, testSuite));
		return kResultTrue;
	}

	tresult PLUGIN_API setEnvironment (ITest* /*environment*/) SMTG_OVERRIDE
	{
		return kNotImplemented;
	}

	int32 getTestCount () const { return static_cast<int32> (tests.size ()); }

	tresult getTest (int32 index, ITest*& _test, std::string& _name) const
	{
		if (auto test = tests.at (index))
		{
			_test = test->test;
			_name = test->name;
			return kResultTrue;
		}
		return kResultFalse;
	}

	tresult getTestSuite (int32 index, ITestSuite*& testSuite, std::string& _name) const
	{
		if (index < 0 || index >= int32 (testSuites.size ()))
			return kInvalidArgument;
		const auto& ts = testSuites[index];
		_name = ts.first;
		testSuite = ts.second;
		return kResultTrue;
	}

	ITestSuite* getTestSuite (FIDString _name) const
	{
		for (const auto& testSuite : testSuites)
		{
			if (testSuite.first == _name)
				return testSuite.second;
		}
		return nullptr;
	}

	const std::string& getName () const { return name; }
	OBJ_METHODS (TestSuite, FObject)
	REFCOUNT_METHODS (FObject)
	DEF_INTERFACES_1 (ITestSuite, FObject)
protected:
	class Test : public FObject
	{
	public:
		Test (FIDString _name, ITest* _test) : name (_name), test (_test) {}

		std::string name;
		IPtr<ITest> test;
	};
	std::string name;
	std::vector<IPtr<Test>> tests;

	using TestSuitePair = std::pair<std::string, IPtr<ITestSuite>>;
	using TestSuiteVector = std::vector<TestSuitePair>;
	TestSuiteVector testSuites;
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
