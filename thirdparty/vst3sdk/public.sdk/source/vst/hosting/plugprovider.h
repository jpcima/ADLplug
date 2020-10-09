//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/hosting/plugprovider.h
// Created by  : Steinberg, 04/2005
// Description : VST 3 Plug-in Provider class
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

#include "public.sdk/source/vst/hosting/module.h"
#include "pluginterfaces/vst/ivsttestplugprovider.h"
#include "base/source/fobject.h"

namespace Steinberg {
namespace Vst {

class IComponent;
class IEditController;
class ConnectionProxy;

//------------------------------------------------------------------------
/** Helper for creating and initializing component.
\ingroup Validator */
//------------------------------------------------------------------------
class PlugProvider : public FObject, public ITestPlugProvider2
{
public:
	using ClassInfo = VST3::Hosting::ClassInfo;
	using PluginFactory = VST3::Hosting::PluginFactory;

	//--- ---------------------------------------------------------------------
	PlugProvider (const PluginFactory& factory, ClassInfo info, bool plugIsGlobal = true);
	~PlugProvider () override;

	//--- from ITestPlugProvider ------------------
	IComponent* PLUGIN_API getComponent () SMTG_OVERRIDE;
	IEditController* PLUGIN_API getController () SMTG_OVERRIDE;
	tresult PLUGIN_API releasePlugIn (IComponent* component, IEditController* controller) SMTG_OVERRIDE;
	tresult PLUGIN_API getSubCategories (IStringResult& result) const SMTG_OVERRIDE
	{
		result.setText (classInfo.subCategoriesString ().data ());
		return kResultTrue;
	}
	tresult PLUGIN_API getComponentUID (FUID& uid) const SMTG_OVERRIDE;

	//--- from ITestPlugProvider2 ------------------
	IPluginFactory* PLUGIN_API getPluginFactory () SMTG_OVERRIDE;

	//--- ---------------------------------------------------------------------
	OBJ_METHODS (PlugProvider, FObject)
	REFCOUNT_METHODS (FObject)
	DEF_INTERFACES_2(ITestPlugProvider, ITestPlugProvider2, FObject)
//------------------------------------------------------------------------
protected:
	bool setupPlugin (FUnknown* hostContext);
	bool connectComponents ();
	bool disconnectComponents ();
	void terminatePlugin ();

	PluginFactory factory;
	IPtr<IComponent> component;
	IPtr<IEditController> controller;
	ClassInfo classInfo;

	OPtr<ConnectionProxy> componentCP;
	OPtr<ConnectionProxy> controllerCP;

	bool plugIsGlobal;
};
}
} // namespaces
