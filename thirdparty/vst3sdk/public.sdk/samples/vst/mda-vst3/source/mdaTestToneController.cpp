/*
 *  mdaTestToneController.cpp
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

#include "mdaTestToneController.h"
#include <cmath>
#include <array>
#include <string>

namespace Steinberg {
namespace Vst {
namespace mda {

#ifdef SMTG_MDA_VST2_COMPATIBILITY
//-----------------------------------------------------------------------------
FUID TestToneController::uid (0x5653456D, 0x6461546D, 0x64612074, 0x65737474);
#else
//-----------------------------------------------------------------------------
FUID TestToneController::uid (0x2FE2D7CB, 0x074C4CF3, 0xAF3608D9, 0xCB3A8E7C);
#endif

//------------------------------------------------------------------------
namespace {


//------------------------------------------------------------------------
constexpr std::array<const char*, 12> noteNames = {
    {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"}};

//------------------------------------------------------------------------
std::string pitchToString (int32 pitch)
{
	assert (pitch >= 0);
	auto octave = pitch / 12u;
	auto note = pitch - (octave * 12u);
	assert (note < noteNames.size ());
	auto noteName = noteNames[note];
	return std::to_string (pitch) + " - " + std::string (noteName) + " " +
	       std::to_string (static_cast<int32> (octave) - 2);
}

//-----------------------------------------------------------------------------
void fillISOFreqParameter (const IPtr<IndexedParameter>& param)
{
	param->setIndexString (0, STR ("20 Hz"));
	param->setIndexString (1, STR ("25 Hz"));
	param->setIndexString (2, STR ("31 Hz"));
	param->setIndexString (3, STR ("40 Hz"));
	param->setIndexString (4, STR ("50 Hz"));
	param->setIndexString (5, STR ("63 Hz"));
	param->setIndexString (6, STR ("80 Hz"));
	param->setIndexString (7, STR ("100 Hz"));
	param->setIndexString (8, STR ("125 Hz"));
	param->setIndexString (9, STR ("160 Hz"));
	param->setIndexString (10, STR ("200 Hz"));
	param->setIndexString (11, STR ("250 Hz"));
	param->setIndexString (12, STR ("310 Hz"));
	param->setIndexString (13, STR ("400 Hz"));
	param->setIndexString (14, STR ("500 Hz"));
	param->setIndexString (15, STR ("630 Hz"));
	param->setIndexString (16, STR ("800 Hz"));
	param->setIndexString (17, STR ("1 kHz"));
	param->setIndexString (18, STR ("1.25 kHz"));
	param->setIndexString (19, STR ("1.6 kHz"));
	param->setIndexString (20, STR ("2.0 kHz"));
	param->setIndexString (21, STR ("2.5 kHz"));
	param->setIndexString (22, STR ("3.1 kHz"));
	param->setIndexString (23, STR ("4 kHz"));
	param->setIndexString (24, STR ("5 kHz"));
	param->setIndexString (25, STR ("6.3 kHz"));
	param->setIndexString (26, STR ("8 kHz"));
	param->setIndexString (27, STR ("10 kHz"));
	param->setIndexString (28, STR ("12.5 kHz"));
	param->setIndexString (29, STR ("16 kHz"));
	param->setIndexString (30, STR ("20 kHz"));
}

//------------------------------------------------------------------------
class CentParameter final : public RangeParameter
{
public:
	CentParameter (const TChar* title, ParamID tag) : RangeParameter (title, tag)
	{
		setPrecision (0);
	}

	ParamValue toPlain (ParamValue normValue) const SMTG_OVERRIDE
	{
		if (normValue > 0.6)
			return (1.25 * normValue - 0.75) * 100.;
		if (normValue < 0.4)
			return (1.25 * normValue - 0.5) * 100.;
		return 0;
	}
	
	ParamValue toNormalized (ParamValue plainValue) const SMTG_OVERRIDE
	{
		if (plainValue < 0.)
			return ((plainValue / 100.) + 0.5) / 1.25;
		if (plainValue > 0.)
			return ((plainValue / 100.) + 0.75) / 1.25;
		return plainValue;
	}
};

//------------------------------------------------------------------------
class FineTuneParameter final : public RangeParameter
{
public:
	FineTuneParameter (const TChar* title, ParamID tag, Parameter* freqParam)
	: RangeParameter (title, tag), freqParameter (freqParam)
	{
		freqParameter->addDependent (this);
		setPrecision (1);
	}
	~FineTuneParameter () noexcept { freqParameter->removeDependent (this); }

	ParamValue toPlain (ParamValue normValue) const SMTG_OVERRIDE
	{
		auto df = 0.;
		if (normValue > 0.6)
		{
			df = (1.25 * normValue - 0.75);
		}
		else if (normValue < 0.4)
		{
			df = (1.25 * normValue - 0.5);
		}
		auto f = std::pow (10.f, 0.1f * (freq + df));
		return f;
	}
	
	ParamValue toNormalized (ParamValue plainValue) const SMTG_OVERRIDE
	{
		return plainValue;
	}

	void PLUGIN_API update (FUnknown* changedUnknown, int32 message) SMTG_OVERRIDE
	{
		if (message != kChanged)
			return;
		auto newFreq = 13. + freqParameter->toPlain (freqParameter->getNormalized ());
		if (newFreq != freq)
		{
			freq = newFreq;
			setNormalized (getNormalized () + std::numeric_limits<float>::epsilon ());
			changed ();
		}
	}

private:
	IPtr<Parameter> freqParameter;
	ParamValue freq;
};

//------------------------------------------------------------------------
} // anonymous

//-----------------------------------------------------------------------------
TestToneController::TestToneController ()
{
}

//-----------------------------------------------------------------------------
TestToneController::~TestToneController ()
{
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API TestToneController::initialize (FUnknown* context)
{
	tresult res = BaseController::initialize (context);
	if (res == kResultTrue)
	{
		IndexedParameter* modeParam = new IndexedParameter (USTRING("Mode"), USTRING(""), 8, 0.15, ParameterInfo::kCanAutomate | ParameterInfo::kIsList, pid (TestToneParam::Mode));
		modeParam->setIndexString (0, UString128("MIDI #"));
		modeParam->setIndexString (1, UString128("IMPULSE"));
		modeParam->setIndexString (2, UString128("WHITE"));
		modeParam->setIndexString (3, UString128("PINK"));
		modeParam->setIndexString (4, UString128("----"));
		modeParam->setIndexString (5, UString128("SINE"));
		modeParam->setIndexString (6, UString128("LOG SWP."));
		modeParam->setIndexString (7, UString128("LOG STEP"));
		modeParam->setIndexString (8, UString128("LIN SWP."));
		parameters.addParameter (modeParam);
		parameters.addParameter (new ScaledParameter (USTRING("Level"), USTRING("dB"), 0, 0.6, ParameterInfo::kCanAutomate, pid (TestToneParam::Level), -60, 0, true));
		IndexedParameter* channelParam = new IndexedParameter (USTRING("Channel"), USTRING("L <> R"), 2, 0.5, ParameterInfo::kCanAutomate | ParameterInfo::kIsList, pid (TestToneParam::Channel));
		channelParam->setIndexString (0, UString128("LEFT"));
		channelParam->setIndexString (1, UString128("CENTER"));
		channelParam->setIndexString (2, UString128("RIGHT"));
		parameters.addParameter (channelParam);
		createF1Parameters ();
		createF2Parameters ();
		parameters.addParameter (f1ProxyParam);
		parameters.addParameter (f2ProxyParam);
		parameters.addParameter (new ScaledParameter (USTRING("Sweep"), USTRING("ms"), 0, 0.5, ParameterInfo::kCanAutomate, pid (TestToneParam::Sweep), 1000, 32000, true));
		parameters.addParameter (new ScaledParameter (USTRING("Thru"), USTRING("dB"), 0, 0.5, ParameterInfo::kCanAutomate, pid (TestToneParam::Thru), -40, 0, true));
		parameters.addParameter (new ScaledParameter (USTRING("0dB ="), USTRING("dBFS"), 0, 0.5, ParameterInfo::kCanAutomate, pid (TestToneParam::ZerodB), -21, 0, true));

		parameters.getParameter (pid (TestToneParam::Mode))->addDependent (this);
	}
	return res;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API TestToneController::terminate ()
{
	f2FineTuneParam->removeDependent (this);
	parameters.getParameter (pid (TestToneParam::Mode))->removeDependent (this);
	return BaseController::terminate ();
}

//-----------------------------------------------------------------------------
void TestToneController::createF1Parameters ()
{
	f1UnusedParam =
	    owned (new RangeParameter (STR ("F1"), pid (TestToneParam::F1), nullptr, 0., 0.));
	f1MIDINotesParam = owned (new IndexedParameter (
	    STR ("F1"), nullptr, 128, 0.5, ParameterInfo::kCanAutomate, pid (TestToneParam::F1)));
	for (auto pitch = 0; pitch < 129; ++pitch)
	{
		auto str = pitchToString (pitch);
		f1MIDINotesParam->setIndexString (pitch, UString128 (str.data ()));
	}
	f1FrequencyParam = owned (new IndexedParameter (
	    STR ("F1"), nullptr, 30, 0.5, ParameterInfo::kCanAutomate, pid (TestToneParam::F1)));
	fillISOFreqParameter (f1FrequencyParam);

	f1LinearFreqParam =
	    owned (new ScaledParameter (STR ("F1"), nullptr, 0, 0.5, ParameterInfo::kCanAutomate,
	                                pid (TestToneParam::F1), 0, 20000, true));
	f1ProxyParam = new ProxyParameter (f1MIDINotesParam);
}

//-----------------------------------------------------------------------------
void TestToneController::createF2Parameters ()
{
	f2UnusedParam =
	    owned (new RangeParameter (STR ("F2"), pid (TestToneParam::F2), nullptr, 0., 0.));

	f2FrequencyParam = owned (new IndexedParameter (
	    STR ("F2"), nullptr, 30, 0.5, ParameterInfo::kCanAutomate, pid (TestToneParam::F2)));
	fillISOFreqParameter (f2FrequencyParam);

	f2LinearFreqParam =
	    owned (new ScaledParameter (STR ("F2"), nullptr, 0, 0.5, ParameterInfo::kCanAutomate,
	                                pid (TestToneParam::F2), 0, 20000, true));

	f2CentParam = owned(new CentParameter (STR ("F2"), pid (TestToneParam::F2)));

	f2FineTuneParam =
	    owned (new FineTuneParameter (STR ("F2"), pid (TestToneParam::F2), f1FrequencyParam));
	f2FineTuneParam->addDependent (this);
	f2ProxyParam = new ProxyParameter (f2UnusedParam);
}

//------------------------------------------------------------------------
void PLUGIN_API TestToneController::update (FUnknown* changedUnknown, int32 message)
{
	if (message != kChanged)
		return;
	if (auto param = FCast<Parameter> (changedUnknown))
	{
		if (param->getInfo ().id == pid (TestToneParam::Mode))
		{
			bool changed = false;
			auto f1paramValue = f1ProxyParam->getNormalized ();
			auto f2paramValue = f2ProxyParam->getNormalized ();
			switch (static_cast<size_t> (param->toPlain (param->getNormalized ())))
			{
				case 0:
				{
					changed = f1ProxyParam->setProxy (f1MIDINotesParam);
					changed |= f2ProxyParam->setProxy (f2CentParam);
					break;
				}
				case 5:
				{
					changed = f1ProxyParam->setProxy (f1FrequencyParam);
					changed |= f2ProxyParam->setProxy (f2FineTuneParam);
					break;
				}
				case 6:
				case 7:
				{
					changed = f1ProxyParam->setProxy (f1FrequencyParam);
					changed |= f2ProxyParam->setProxy (f2FrequencyParam);
					break;
				}
				case 8:
				{
					changed = f1ProxyParam->setProxy (f1LinearFreqParam);
					changed |= f2ProxyParam->setProxy (f2LinearFreqParam);
					break;
				}
				default:
				{
					changed = f1ProxyParam->setProxy (f1UnusedParam);
					changed |= f2ProxyParam->setProxy (f2UnusedParam);
					break;
				}
			}
			if (changed)
			{
				f1ProxyParam->setNormalized (f1paramValue);
				f2ProxyParam->setNormalized (f2paramValue);
				if (componentHandler)
					componentHandler->restartComponent (kParamTitlesChanged | kParamValuesChanged);
			}
		}
		else if (auto param = FCast<FineTuneParameter> (changedUnknown))
		{
			if (componentHandler)
				componentHandler->restartComponent (kParamValuesChanged);
		}
	}
	BaseController::update (changedUnknown, message);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API TestToneController::getParamStringByValue (ParamID tag, ParamValue valueNormalized, String128 string)
{
	UString128 result;
	switch (param (tag))
	{
		case TestToneParam::Thru:
			if (valueNormalized == 0)
			{
				result.fromAscii ("OFF");
				break;
			}
		default:
			return BaseController::getParamStringByValue (tag, valueNormalized, string);
	}
	result.copyTo (string, 128);
	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API TestToneController::getParamValueByString (ParamID tag, TChar* string, ParamValue& valueNormalized)
{
	// TODO
	return BaseController::getParamValueByString (tag, string, valueNormalized);
	
	/*
	switch (tag)
		{
			default:
				return BaseController::getParamValueByString (tag, string, valueNormalized);
		}
		return kResultFalse;*/
	
}

}}} // namespaces
