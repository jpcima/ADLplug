//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Validator
// Filename    : usediids.cpp
// Created by  : Steinberg 09.2008
// Description : Interface symbols file
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2019, Steinberg Media Technologies GmbH, All Rights Reserved
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
//#define INIT_CLASS_IID
// This macro definition modifies the behavior of DECLARE_CLASS_IID (funknown.h)
// and produces the actual symbols for all interface identifiers.
// It must be defined before including the interface headers and
// in only one source file!
//------------------------------------------------------------------------
//#define INIT_CLASS_IID

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivsthostapplication.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstpluginterfacesupport.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstunits.h"

namespace Steinberg {
DEF_CLASS_IID (Vst::IAttributeList)
DEF_CLASS_IID (Vst::IAudioProcessor)
DEF_CLASS_IID (Vst::IEditController)
DEF_CLASS_IID (Vst::IEditController2)
DEF_CLASS_IID (Vst::IComponent)
DEF_CLASS_IID (Vst::IComponentHandler)
DEF_CLASS_IID (Vst::IConnectionPoint)
DEF_CLASS_IID (Vst::IEventList)
DEF_CLASS_IID (Vst::IHostApplication)
DEF_CLASS_IID (Vst::IMessage)
DEF_CLASS_IID (Vst::IMidiMapping)
DEF_CLASS_IID (Vst::IParameterChanges)
DEF_CLASS_IID (Vst::IParamValueQueue)
DEF_CLASS_IID (Vst::IPlugInterfaceSupport)
DEF_CLASS_IID (Vst::IProgramListData)
DEF_CLASS_IID (Vst::IStreamAttributes)
DEF_CLASS_IID (Vst::IVst3ToAUWrapper)
DEF_CLASS_IID (Vst::IUnitData)
DEF_CLASS_IID (Vst::IUnitInfo)
}
