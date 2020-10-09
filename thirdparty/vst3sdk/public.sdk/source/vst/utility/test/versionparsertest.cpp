//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/utility/test/versionparsertest.cpp
// Created by  : Steinberg, 12/2019
// Description : Test version parser
// Flags       : clang-format SMTGSequencer
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

#include "public.sdk/source/vst/utility/test/versionparsertest.h"
#include "public.sdk/source/vst/utility/versionparser.h"
#include "pluginterfaces/base/fstrdefs.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
bool PLUGIN_API VersionParserTest::setup ()
{
	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API VersionParserTest::run (ITestResult* testResult)
{
	auto version = VST3::Version::parse ("SDK 3.7");
	if (version.getMajor () != 3 || version.getMinor () != 7 || version.getSub () != 0 ||
	    version.getBuildnumber () != 0)
	{
		testResult->addErrorMessage (STR ("Parsing 'SDK 3.7' failed"));
		return false;
	}

	version = VST3::Version::parse ("3.7.1.38");
	if (version.getMajor () != 3 || version.getMinor () != 7 || version.getSub () != 1 ||
	    version.getBuildnumber () != 38)
	{
		testResult->addErrorMessage (STR ("Parsing '3.7.1.38' failed"));
		return false;
	}

	version = VST3::Version::parse ("SDK 3.7 Prerelease");
	if (version.getMajor () != 3 || version.getMinor () != 7 || version.getSub () != 0 ||
	    version.getBuildnumber () != 0)
	{
		testResult->addErrorMessage (STR ("Parsing 'SDK 3.7 Prerelease' failed"));
		return false;
	}

	version = VST3::Version::parse ("SDK 3.7-99");
	if (version.getMajor () != 3 || version.getMinor () != 7 || version.getSub () != 0 ||
	    version.getBuildnumber () != 0)
	{
		testResult->addErrorMessage (STR ("Parsing 'SDK 3.7-99' failed"));
		return false;
	}

	version = VST3::Version::parse ("No version at all");
	if (version.getMajor () != 0 || version.getMinor () != 0 || version.getSub () != 0 ||
	    version.getBuildnumber () != 0)
	{
		testResult->addErrorMessage (STR ("Parsing 'No version at all' failed"));
		return false;
	}

	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API VersionParserTest::teardown ()
{
	return true;
}

//------------------------------------------------------------------------
const tchar* PLUGIN_API VersionParserTest::getDescription ()
{
	return STR ("VersionParser Tests");
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
