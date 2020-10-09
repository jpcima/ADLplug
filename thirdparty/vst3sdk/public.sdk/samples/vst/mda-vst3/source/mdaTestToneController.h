/*
 *  mdaTestToneController.h
 *  mda-vst3
 *
 *  Created by Arne Scheffler on 6/14/08.
 *
 *  mda VST Plug-ins
 *
 *  Copyright (c) 2008 Paul Kellett
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include "mdaBaseController.h"
#include "mdaTestToneProcessor.h"
#include <cassert>

namespace Steinberg {
namespace Vst {
namespace mda {

class ProxyParameter;

//-----------------------------------------------------------------------------
class TestToneController : public BaseController
{
public:
	TestToneController ();
	~TestToneController ();
	
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API terminate () SMTG_OVERRIDE;

	tresult PLUGIN_API getParamStringByValue (ParamID tag, ParamValue valueNormalized, String128 string) SMTG_OVERRIDE;
	tresult PLUGIN_API getParamValueByString (ParamID tag, TChar* string, ParamValue& valueNormalized) SMTG_OVERRIDE;

//-----------------------------------------------------------------------------
	static FUnknown* createInstance (void*) { return (IEditController*)new TestToneController; }
	static FUID uid;
private:
	ParamID pid (TestToneParam p) const { return static_cast<ParamID> (p); }
	TestToneParam param (ParamID p) const { return static_cast<TestToneParam> (p); }
	void createF1Parameters ();
	void createF2Parameters ();
	void PLUGIN_API update (FUnknown* changedUnknown, int32 message) SMTG_OVERRIDE;

	IPtr<ProxyParameter> f1ProxyParam;
	IPtr<Parameter> f1UnusedParam;
	IPtr<IndexedParameter> f1MIDINotesParam;
	IPtr<IndexedParameter> f1FrequencyParam;
	IPtr<ScaledParameter> f1LinearFreqParam;

	IPtr<ProxyParameter> f2ProxyParam;
	IPtr<Parameter> f2UnusedParam;
	IPtr<IndexedParameter> f2FrequencyParam;
	IPtr<ScaledParameter> f2LinearFreqParam;
	IPtr<Parameter> f2CentParam;
	IPtr<Parameter> f2FineTuneParam;
};

//------------------------------------------------------------------------
class ProxyParameter final : public Parameter
{
public:
	ProxyParameter (Parameter* p) : parameter (p) { assert (p); }

	bool setProxy (Parameter* p)
	{
		assert (p);
		if (p == parameter)
			return false;
		parameter = p;
		return true;
	}

	Parameter* getProxy () const { return parameter; }

	// overrides
	const ParameterInfo& getInfo () const SMTG_OVERRIDE { return parameter->getInfo (); }
	ParameterInfo& getInfo () SMTG_OVERRIDE { return parameter->getInfo (); }
	void setUnitID (UnitID id) SMTG_OVERRIDE { parameter->setUnitID (id); }
	UnitID getUnitID () SMTG_OVERRIDE { return parameter->getUnitID (); }
	bool setNormalized (ParamValue v) SMTG_OVERRIDE
	{
		if (parameter->setNormalized (v))
		{
			Parameter::setNormalized (parameter->getNormalized ());
			return true;
		}
		return false;
	}
	void toString (ParamValue valueNormalized, String128 string) const SMTG_OVERRIDE
	{
		parameter->toString (valueNormalized, string);
	}
	bool fromString (const TChar* string, ParamValue& valueNormalized) const SMTG_OVERRIDE
	{
		return parameter->fromString (string, valueNormalized);
	}
	ParamValue toPlain (ParamValue valueNormalized) const SMTG_OVERRIDE
	{
		return parameter->toPlain (valueNormalized);
	}
	ParamValue toNormalized (ParamValue plainValue) const SMTG_OVERRIDE
	{
		return parameter->toNormalized (plainValue);
	}
	int32 getPrecision () const SMTG_OVERRIDE { return parameter->getPrecision (); }
	void setPrecision (int32 val) SMTG_OVERRIDE { parameter->setPrecision (val); }

private:
	IPtr<Parameter> parameter;
};

}}} // namespaces
