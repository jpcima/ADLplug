/*
 *  mdaDX10Processor.h
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

#include "mdaBaseProcessor.h"

namespace Steinberg {
namespace Vst {
namespace mda {

//-----------------------------------------------------------------------------
class DX10Processor : public BaseProcessor
{
public:

	enum {
		NPARAMS=16,
		kNumPrograms = 32
	};

	DX10Processor ();
	~DX10Processor ();
	
	int32 getVst2UniqueId () const SMTG_OVERRIDE { return 'MDAx'; }

	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;

	void doProcessing (ProcessData& data) SMTG_OVERRIDE;
	
	bool hasProgram () const SMTG_OVERRIDE { return true; }
	Steinberg::uint32 getCurrentProgram () const SMTG_OVERRIDE { return currentProgram; }
	Steinberg::uint32 getNumPrograms () const SMTG_OVERRIDE { return kNumPrograms; }
	void setCurrentProgram (Steinberg::uint32 val) SMTG_OVERRIDE;
	void setCurrentProgramNormalized (ParamValue val) SMTG_OVERRIDE;

//-----------------------------------------------------------------------------
	static FUnknown* createInstance (void*) { return (IAudioProcessor*)new DX10Processor; }
	static FUID uid;
//-----------------------------------------------------------------------------

	static float programParams[][NPARAMS];

	struct VOICE  //voice state
	{
		float env;  //carrier envelope
		float dmod; //modulator oscillator 
		float mod0;
		float mod1;
		float menv; //modulator envelope
		float mlev; //modulator target level
		float mdec; //modulator envelope decay
		float car;  //carrier oscillator
		float dcar;
		float cenv; //smoothed env
		float catt; //smoothing
		float cdec; //carrier envelope decay
		int32  note; //remember what note triggered this
	};

protected:
	void checkSilence (ProcessData& data) SMTG_OVERRIDE {}
	void setParameter (ParamID index, ParamValue newValue, int32 sampleOffset) SMTG_OVERRIDE;
	void processEvents (IEventList* events) SMTG_OVERRIDE;
	void recalculate () SMTG_OVERRIDE;
	void noteOn(int32 note, int32 velocity);

	float Fs;

	enum {
		EVENTBUFFER = 120,
		EVENTS_DONE = 99999999,
		NVOICES = 8,       //max polyphony
		SUSTAIN = 128
	};
	int32 notes[EVENTBUFFER + 8];  //list of delta|note|velocity for current block

	///global internal variables
	VOICE voice[NVOICES];
	int32 sustain, activevoices, K;

	float tune, rati, ratf, ratio; //modulator ratio
	float catt, cdec, crel;        //carrier envelope
	float depth, dept2, mdec, mrel; //modulator envelope
	float lfo0, lfo1, dlfo, modwhl, MW, pbend, velsens, volume, vibrato; //LFO and CC
	float rich, modmix;

	Steinberg::uint32 currentProgram;
};

}}} // namespaces
