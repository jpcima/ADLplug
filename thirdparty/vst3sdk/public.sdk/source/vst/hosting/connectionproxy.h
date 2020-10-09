//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/utility/connectionproxy.h
// Created by  : Steinberg, 04/2020
// Description : VST 3 Plug-in connection class
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

#include "base/source/fobject.h"
#include "pluginterfaces/vst/ivstmessage.h"
#include "public.sdk/source/common/threadchecker.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
/** Helper for creating and initializing component.
\ingroup Helper */
//------------------------------------------------------------------------
class ConnectionProxy : public FObject, public IConnectionPoint
{
public:
	ConnectionProxy (IConnectionPoint* srcConnection);
	~ConnectionProxy () override;

	//--- from IConnectionPoint
	tresult PLUGIN_API connect (IConnectionPoint* other) override;
	tresult PLUGIN_API disconnect (IConnectionPoint* other) override;
	tresult PLUGIN_API notify (IMessage* message) override;

	bool disconnect ();

	OBJ_METHODS (ConnectionProxy, FObject)
	REFCOUNT_METHODS (FObject)
	DEF_INTERFACES_1 (IConnectionPoint, FObject)

protected:
	std::unique_ptr<ThreadChecker> threadChecker {ThreadChecker::create ()};

	IPtr<IConnectionPoint> srcConnection;
	IPtr<IConnectionPoint> dstConnection;
};
}
} // namespaces
