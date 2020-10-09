//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : validator.h
// Created by  : Steinberg, 04/2005
// Description : VST 3 Plug-in Validator class
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

#include "public.sdk/source/vst/hosting/hostclasses.h"
#include "public.sdk/source/vst/hosting/module.h"
#include "public.sdk/source/vst/hosting/pluginterfacesupport.h"
#include "public.sdk/source/vst/testsuite/vsttestsuite.h"
#include "pluginterfaces/base/ipluginbase.h"
#include "pluginterfaces/test/itest.h"
#include <iosfwd>

namespace Steinberg {
namespace Vst {

class IComponent;
class IEditController;

class VstModule;
class TestBase;
class TestSuite;

//------------------------------------------------------------------------
/** Main Class of Validator.
\ingroup Validator */
//------------------------------------------------------------------------
class Validator : public FObject, public ITestResult, public IHostApplication
{
public:
//------------------------------------------------------------------------
	Validator (int argc, char* argv[]);
	~Validator () override;

	int run ();

//------------------------------------------------------------------------
	OBJ_METHODS (Validator, FObject)
	REFCOUNT_METHODS (FObject)

	tresult PLUGIN_API queryInterface (const char* _iid, void** obj) override;
//------------------------------------------------------------------------
protected:
	using Module = VST3::Hosting::Module;

	// ITestResult
	void PLUGIN_API addErrorMessage (const tchar* msg) override;
	void PLUGIN_API addMessage (const tchar* msg) override;

	// IHostApplication
	tresult PLUGIN_API getName (String128 name) override;
	tresult PLUGIN_API createInstance (TUID cid, TUID iid, void** obj) override;

	IPtr<TestSuite> createTests (ITestPlugProvider* plugProvider, const ConstString& plugName,
	                             bool extensive);
	void addTest (ITestSuite* testSuite, TestBase* test);
	void runTestSuite (TestSuite* suite, FIDString nameFilter = nullptr);

	struct ModuleTestConfig
	{
		ModuleTestConfig (bool useGlobalInstance, bool useExtensiveTests,
		                  std::string& customTestComponentPath, std::string& testSuiteName,
		                  VST3::Optional<VST3::UID>&& testProcessor)
		: useGlobalInstance (useGlobalInstance)
		, useExtensiveTests (useExtensiveTests)
		, customTestComponentPath (customTestComponentPath)
		, testSuiteName (testSuiteName)
		, testProcessor (std::move (testProcessor))
		{
		}

		bool useGlobalInstance {true};
		bool useExtensiveTests {false};
		std::string customTestComponentPath;
		std::string testSuiteName;
		VST3::Optional<VST3::UID> testProcessor;
	};

	void testModule (Module::Ptr module, const ModuleTestConfig& config);

	int argc;
	char** argv;

	IPtr<PlugInterfaceSupport> mPlugInterfaceSupport;

	int32 numTestsFailed {0};
	int32 numTestsPassed {0};

	std::ostream* infoStream {nullptr};
	std::ostream* errorStream {nullptr};
};

//------------------------------------------------------------------------
/** This handles the plug-in factory in its library.
\ingroup Validator */
//------------------------------------------------------------------------
class VstModule : public FObject
{
public:
//------------------------------------------------------------------------
	VstModule (const char* path);
	~VstModule () override;

	bool isValid () const;
	IPluginFactory* getFactory () { return factory; }

//------------------------------------------------------------------------
	OBJ_METHODS (VstModule, FObject)
//------------------------------------------------------------------------
protected:
	void* libHandle;
	IPluginFactory* factory;
};
}
} // namespaces
