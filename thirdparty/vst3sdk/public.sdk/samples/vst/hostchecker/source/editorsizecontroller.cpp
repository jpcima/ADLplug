//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostchecker/source/hostchecker.h
// Created by  : Steinberg, 04/2012
// Description :
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

#include "editorsizecontroller.h"
#include "vstgui/lib/controls/ccontrol.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
EditorSizeController::EditorSizeController (EditController* /*editController*/,
                                            const SizeFunc& sizeFunc, double currentSizeFactor)
: sizeFunc (sizeFunc)
{
	const auto kMaxValue = static_cast<ParamValue> (kSizeFactors.size () - 1);
	sizeParameter =
	    new RangeParameter (STR ("EditorSize"), kSizeParamTag, nullptr, 0, kMaxValue, 1, kMaxValue);
	sizeParameter->setMin (0);
	sizeParameter->setMax (kSizeFactors.size () - 1);

	setSizeFactor (currentSizeFactor);

	sizeParameter->addDependent (this);
}

//------------------------------------------------------------------------
EditorSizeController::~EditorSizeController ()
{
	if (sizeParameter)
		sizeParameter->removeDependent (this);
}

//------------------------------------------------------------------------
void PLUGIN_API EditorSizeController::update (FUnknown* changedUnknown, int32 /*message*/)
{
	auto* param = FCast<Parameter> (changedUnknown);
	if (param && param->getInfo ().id == kSizeParamTag)
	{
		size_t index = static_cast<size_t> (param->toPlain (param->getNormalized ()));
		if (sizeFunc)
			sizeFunc (kSizeFactors.at (index));
	}
}

//------------------------------------------------------------------------
VSTGUI::CView* EditorSizeController::verifyView (VSTGUI::CView* view,
                                                 const VSTGUI::UIAttributes& /*attributes*/,
                                                 const VSTGUI::IUIDescription* /*description*/)
{
	auto* control = dynamic_cast<VSTGUI::CControl*> (view);
	if (control)
	{
		sizeControl = control;
		sizeControl->setValueNormalized (sizeParameter->getNormalized ());
		sizeControl->setListener (this);
		sizeParameter->deferUpdate ();
	}
	return view;
}

//------------------------------------------------------------------------
void EditorSizeController::valueChanged (VSTGUI::CControl* pControl)
{
	if (!pControl)
		return;

	auto normValue = static_cast<ParamValue> (pControl->getValue ());
	sizeParameter->setNormalized (normValue);
}

//------------------------------------------------------------------------
void EditorSizeController::controlBeginEdit (VSTGUI::CControl* pControl)
{
	if (!pControl)
		return;
}

//------------------------------------------------------------------------
void EditorSizeController::controlEndEdit (VSTGUI::CControl* pControl)
{
	if (!pControl)
		return;
}

//------------------------------------------------------------------------
void EditorSizeController::setSizeFactor (double factor)
{
	if (!sizeParameter)
		return;
	auto iter = std::find (kSizeFactors.begin (), kSizeFactors.end (), factor);
	if (iter != kSizeFactors.end ())
	{
		sizeParameter->setNormalized (sizeParameter->toNormalized (iter - kSizeFactors.begin ()));
		if (sizeControl)
			sizeControl->setValueNormalized (sizeParameter->getNormalized ());
	}
}

//------------------------------------------------------------------------

} // Vst
} // Steinberg
