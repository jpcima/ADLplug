//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/aaxwrapper/aaxwrapper_gui.cpp
// Created by  : Steinberg, 08/2017
// Description : VST 3 -> AAX Wrapper
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

/// \cond ignore

#include "aaxwrapper_gui.h"
#include "aaxwrapper.h"
#include "aaxwrapper_parameters.h"

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace Steinberg::Base::Thread;

//------------------------------------------------------------------------
void AAXWrapper_GUI::CreateViewContainer ()
{
	if (GetViewContainerType () == AAX_eViewContainer_Type_HWND ||
	    GetViewContainerType () == AAX_eViewContainer_Type_NSView)
	{
		mHWND = this->GetViewContainerPtr ();
		AAXWrapper* wrapper =
		    static_cast<AAXWrapper_Parameters*> (GetEffectParameters ())->getWrapper ();

		FGuard guard (wrapper->mSyncCalls);
		wrapper->setGUI (this);
		if (auto* editor = wrapper->getEditor ())
			editor->_open (mHWND);
		// if ( mHWND && mPlugInHWND )
		//	SetParent( mPlugInHWND, mHWND );
	}
}

//------------------------------------------------------------------------
AAX_Result AAXWrapper_GUI::GetViewSize (AAX_Point* oEffectViewSize) const
{
	oEffectViewSize->horz = 1024;
	oEffectViewSize->vert = 768;

	auto* that = const_cast<AAXWrapper_GUI*> (this);
	auto* params = static_cast<AAXWrapper_Parameters*> (that->GetEffectParameters ());
	int32 width, height;
	if (params->getWrapper ()->getEditorSize (width, height))
	{
		oEffectViewSize->horz = width;
		oEffectViewSize->vert = height;
	}
	return AAX_SUCCESS;
}

//------------------------------------------------------------------------
AAX_Result AAXWrapper_GUI::SetControlHighlightInfo (AAX_CParamID iParameterID,
                                                    AAX_CBoolean iIsHighlighted,
                                                    AAX_EHighlightColor iColor)
{
	// TODO
	return AAX_SUCCESS;
}

//------------------------------------------------------------------------
void AAXWrapper_GUI::DeleteViewContainer ()
{
	AAXWrapper* wrapper =
	    static_cast<AAXWrapper_Parameters*> (GetEffectParameters ())->getWrapper ();
	wrapper->setGUI (nullptr);

	if (auto* editor = wrapper->getEditor ())
		editor->_close ();
}

//------------------------------------------------------------------------
// METHOD:	CreateViewContents
//------------------------------------------------------------------------
void AAXWrapper_GUI::CreateViewContents ()
{
}

/// \endcond
