//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : plugfactory.cpp
// Created by  : Steinberg, 02/2020
// Description : Panner Example for VST 3
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

#include "public.sdk/source/main/pluginfactory.h"

#include "../include/plugcontroller.h"	// for createInstance
#include "../include/plugprocessor.h"	// for createInstance
#include "../include/plugids.h"			// for uids
#include "../include/version.h"			// for version and naming

#define stringSubCategory Vst::PlugType::kSpatialFx	// Subcategory for this plug-in (to be changed if needed, see PlugType in ivstaudioprocessor.h)

BEGIN_FACTORY_DEF (stringCompanyName, stringCompanyWeb,	stringCompanyEmail)

	DEF_CLASS2 (INLINE_UID_FROM_FUID(Steinberg::Panner::MyProcessorUID),
				PClassInfo::kManyInstances,	// cardinality  
				kVstAudioEffectClass,	// the component category (do not changed this)
				stringPluginName,		// here the plug-in name (to be changed)
				Vst::kDistributable,	// means that component and controller could be distributed on different computers
				stringSubCategory,		// Subcategory for this plug-in (to be changed)
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				Steinberg::Panner::PlugProcessor::createInstance)	// function pointer called when this component should be instantiated

	DEF_CLASS2 (INLINE_UID_FROM_FUID(Steinberg::Panner::MyControllerUID),
				PClassInfo::kManyInstances,  // cardinality   
				kVstComponentControllerClass,// the Controller category (do not changed this)
				stringPluginName "Controller",	// controller name (could be the same than component name)
				0,						// not used here
				"",						// not used here
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				Steinberg::Panner::PlugController::createInstance)// function pointer called when this component should be instantiated

END_FACTORY

//------------------------------------------------------------------------
//  Module init/exit
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// called after library was loaded
bool InitModule ()
{
	return true;
}

//------------------------------------------------------------------------
// called after library is unloaded
bool DeinitModule ()
{
	return true;
}
