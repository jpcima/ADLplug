//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/testbase.h
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

#include "pluginterfaces/test/itest.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivsttestplugprovider.h"
#include <atomic>
#include <cstdlib>
#include <string>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
void addMessage (ITestResult* testResult, const std::u16string& str);
void addMessage (ITestResult* testResult, const tchar* str);
void addErrorMessage (ITestResult* testResult, const tchar* str);
void addErrorMessage (ITestResult* testResult, const std::u16string& str);
std::u16string printf (const char8* format, ...);

//------------------------------------------------------------------------
#define DECLARE_VSTTEST(name) \
	const char* getName () const SMTG_OVERRIDE { return name; }

//------------------------------------------------------------------------
struct TestingPluginContext
{
	static FUnknown* get () { return instance ().context; }
	static void set (FUnknown* context) { instance ().context = context; }

private:
	static TestingPluginContext& instance ()
	{
		static TestingPluginContext gInstance;
		return gInstance;
	}
	FUnknown* context {nullptr};
};

//------------------------------------------------------------------------
struct TestDefaults
{
	int32 numIterations {20};
	int32 defaultSampleRate {44100};
	int32 defaultBlockSize {64};
	int32 maxBlockSize {8192};
	int32 buffersAreEqual {0};
	int32 numAudioBlocksToProcess {3};
	uint64 channelIsSilent {1};

	static TestDefaults& instance ()
	{
		static TestDefaults gInstance;
		return gInstance;
	}
};

//------------------------------------------------------------------------
/** Test Helper.
\ingroup TestClass */
//------------------------------------------------------------------------
class TestBase : public ITest
{
public:
//------------------------------------------------------------------------
	TestBase (ITestPlugProvider* plugProvider);
	virtual ~TestBase ();

	virtual const char* getName () const = 0;
	DECLARE_FUNKNOWN_METHODS

	bool PLUGIN_API setup () SMTG_OVERRIDE;
	bool PLUGIN_API run (ITestResult* /*testResult*/) SMTG_OVERRIDE = 0;
	bool PLUGIN_API teardown () SMTG_OVERRIDE;

	virtual bool activateMainIOBusses (bool val);
	virtual void printTestHeader (ITestResult* testResult);
//------------------------------------------------------------------------
protected:
	ITestPlugProvider* plugProvider;
	IComponent* vstPlug;
	IEditController* controller;

private:
	TestBase ();
};

using ProcessSampleSize = int32;

//------------------------------------------------------------------------
/** Test Helper.
\ingroup TestClass */
//------------------------------------------------------------------------
class TestEnh : public TestBase
{
public:
//------------------------------------------------------------------------
	TestEnh (ITestPlugProvider* plugProvider, ProcessSampleSize sampl);
	~TestEnh () override;

	enum AudioDefaults
	{
		kBlockSize = 64,
		kMaxSamplesPerBlock = 8192,
		kSampleRate = 44100,
	};

	bool PLUGIN_API setup () SMTG_OVERRIDE;
	bool PLUGIN_API teardown () SMTG_OVERRIDE;
//------------------------------------------------------------------------
protected:
	// interfaces
	IAudioProcessor* audioEffect;

	ProcessSetup processSetup;
};

//------------------------------------------------------------------------
/** AutomationTest helper classes.
\ingroup TestClass */
class ParamPoint
{
public:
	ParamPoint () : offsetSamples (-1), value (0.), read (false) {}
	void set (int32 _offsetSamples, double _value)
	{
		offsetSamples = _offsetSamples;
		value = _value;
	}
	void get (int32& _offsetSamples, double& _value)
	{
		_offsetSamples = offsetSamples;
		_value = value;
		read = true;
	}
	bool wasRead () const { return read; }

private:
	int32 offsetSamples;
	double value;
	bool read;
};

//------------------------------------------------------------------------
/** AutomationTest helper classes: implementation of IParamValueQueue.
\ingroup TestClass */
class ParamChanges : public IParamValueQueue
{
public:
	DECLARE_FUNKNOWN_METHODS

	ParamChanges ();
	virtual ~ParamChanges ();

	void init (ParamID _id, int32 _numPoints);
	bool setPoint (int32 index, int32 offsetSamples, double value);
	void resetPoints ();
	int32 getProcessedFrames () const;
	void setProcessedFrames (int32 amount);
	bool havePointsBeenRead (bool atAll);

	//---for IParamValueQueue-------------------------
	ParamID PLUGIN_API getParameterId () SMTG_OVERRIDE;
	int32 PLUGIN_API getPointCount () SMTG_OVERRIDE;
	tresult PLUGIN_API getPoint (int32 index, int32& offsetSamples, double& value) SMTG_OVERRIDE;
	tresult PLUGIN_API addPoint (int32 /*offsetSamples*/, double /*value*/,
	                             int32& /*index*/) SMTG_OVERRIDE;
//---------------------------------------------------------

private:
	ParamID id = kNoParamId;
	int32 numPoints = 0;
	int32 numUsedPoints = 0;
	int32 processedFrames = 0;
	ParamPoint* points = nullptr;
};

//------------------------------------------------------------------------
class StringResult final : public IStringResult
{
public:
	const std::string& get () const { return data; }
	void PLUGIN_API setText (const char8* text) override { data = text; }

	tresult PLUGIN_API queryInterface (const TUID _iid, void** obj) override
	{
		QUERY_INTERFACE (_iid, obj, FUnknown::iid, IStringResult)
		QUERY_INTERFACE (_iid, obj, IStringResult::iid, IStringResult)
		*obj = nullptr;
		return kNoInterface;
	}
	uint32 PLUGIN_API addRef () override { return ++__refCount; }
	uint32 PLUGIN_API release () override
	{
		if (--__refCount == 0)
		{
			delete this;
			return 0;
		}
		return __refCount;
	}

private:
	std::string data;
	std::atomic<uint32> __refCount {0};
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
