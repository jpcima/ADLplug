//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/vsttestsuite.h
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

#include "public.sdk/source/vst/testsuite/bus/busactivation.h"
#include "public.sdk/source/vst/testsuite/bus/busconsistency.h"
#include "public.sdk/source/vst/testsuite/bus/businvalidindex.h"
#include "public.sdk/source/vst/testsuite/bus/checkaudiobusarrangement.h"
#include "public.sdk/source/vst/testsuite/bus/scanbusses.h"
#include "public.sdk/source/vst/testsuite/bus/sidechainarrangement.h"
#include "public.sdk/source/vst/testsuite/general/editorclasses.h"
#include "public.sdk/source/vst/testsuite/general/midilearn.h"
#include "public.sdk/source/vst/testsuite/general/midimapping.h"
#include "public.sdk/source/vst/testsuite/general/parameterfunctionname.h"
#include "public.sdk/source/vst/testsuite/general/scanparameters.h"
#include "public.sdk/source/vst/testsuite/general/suspendresume.h"
#include "public.sdk/source/vst/testsuite/general/terminit.h"
#include "public.sdk/source/vst/testsuite/noteexpression/keyswitch.h"
#include "public.sdk/source/vst/testsuite/noteexpression/noteexpression.h"
#include "public.sdk/source/vst/testsuite/processing/automation.h"
#include "public.sdk/source/vst/testsuite/processing/process.h"
#include "public.sdk/source/vst/testsuite/processing/processcontextrequirements.h"
#include "public.sdk/source/vst/testsuite/processing/processformat.h"
#include "public.sdk/source/vst/testsuite/processing/processinputoverwriting.h"
#include "public.sdk/source/vst/testsuite/processing/processtail.h"
#include "public.sdk/source/vst/testsuite/processing/processthreaded.h"
#include "public.sdk/source/vst/testsuite/processing/silenceflags.h"
#include "public.sdk/source/vst/testsuite/processing/silenceprocessing.h"
#include "public.sdk/source/vst/testsuite/processing/speakerarrangement.h"
#include "public.sdk/source/vst/testsuite/processing/variableblocksize.h"
#include "public.sdk/source/vst/testsuite/state/bypasspersistence.h"
#include "public.sdk/source/vst/testsuite/state/invalidstatetransition.h"
#include "public.sdk/source/vst/testsuite/state/repeatidenticalstatetransition.h"
#include "public.sdk/source/vst/testsuite/state/validstatetransition.h"
#include "public.sdk/source/vst/testsuite/unit/checkunitstructure.h"
#include "public.sdk/source/vst/testsuite/unit/scanprograms.h"
#include "public.sdk/source/vst/testsuite/unit/scanunits.h"
