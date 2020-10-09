//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/again/source/againuimessagecontroller.h
// Created by  : Steinberg, 04/2005
// Description : AGain UI Message Controller
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

#pragma once

#include "vstgui/lib/iviewlistener.h"
#include "vstgui/uidescription/icontroller.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// AGainUIMessageController
//------------------------------------------------------------------------
template <typename ControllerType>
class AGainUIMessageController : public VSTGUI::IController, public VSTGUI::ViewListenerAdapter
{
public:
	enum Tags
	{
		kSendMessageTag = 1000
	};

	AGainUIMessageController (ControllerType* againController) : againController (againController), textEdit (nullptr)
	{
	}
	~AGainUIMessageController () override
	{
		if (textEdit)
			viewWillDelete (textEdit);
		againController->removeUIMessageController (this);
	}

	void setMessageText (String128 msgText)
	{
		if (!textEdit)
			return;
		String str (msgText);
		str.toMultiByte (kCP_Utf8);
		textEdit->setText (str.text8 ());
	}

private:
	using CControl = VSTGUI::CControl;
	using CView = VSTGUI::CView;
	using CTextEdit = VSTGUI::CTextEdit;
	using UTF8String = VSTGUI::UTF8String;
	using UIAttributes = VSTGUI::UIAttributes;
	using IUIDescription = VSTGUI::IUIDescription;

	//--- from IControlListener ----------------------
	void valueChanged (CControl* /*pControl*/) override {}
	void controlBeginEdit (CControl* /*pControl*/) override {}
	void controlEndEdit (CControl* pControl) override
	{
		if (pControl->getTag () == kSendMessageTag)
		{
			if (pControl->getValueNormalized () > 0.5f)
			{
				againController->sendTextMessage (textEdit->getText ().data ());
				pControl->setValue (0.f);
				pControl->invalid ();

				//---send a binary message
				if (IPtr<IMessage> message = owned (againController->allocateMessage ()))
				{
					message->setMessageID ("BinaryMessage");
					uint32 size = 100;
					char8 data[100];
					memset (data, 0, size * sizeof (char));
					// fill my data with dummy stuff
					for (uint32 i = 0; i < size; i++)
						data[i] = i;
					message->getAttributes ()->setBinary ("MyData", data, size);
					againController->sendMessage (message);
				}
			}
		}
	}
	//--- from IControlListener ----------------------
	//--- is called when a view is created -----
	CView* verifyView (CView* view, const UIAttributes& /*attributes*/,
	                   const IUIDescription* /*description*/) override
	{
		if (CTextEdit* te = dynamic_cast<CTextEdit*> (view))
		{
			// this allows us to keep a pointer of the text edit view
			textEdit = te;

			// add this as listener in order to get viewWillDelete and viewLostFocus calls
			textEdit->registerViewListener (this);

			// initialize it content
			String str (againController->getDefaultMessageText ());
			str.toMultiByte (kCP_Utf8);
			textEdit->setText (str.text8 ());
		}
		return view;
	}
	//--- from IViewListenerAdapter ----------------------
	//--- is called when a view will be deleted: the editor is closed -----
	void viewWillDelete (CView* view) override
	{
		if (dynamic_cast<CTextEdit*> (view) == textEdit)
		{
			textEdit->unregisterViewListener (this);
			textEdit = nullptr;
		}
	}
	//--- is called when the view is loosing the focus -----------------
	void viewLostFocus (CView* view) override
	{
		if (dynamic_cast<CTextEdit*> (view) == textEdit)
		{
			// save the last content of the text edit view
			const UTF8String& text = textEdit->getText ();
			String128 messageText;
			String str;
			str.fromUTF8 (text.data ());
			str.copyTo (messageText, 0, 128);
			againController->setDefaultMessageText (messageText);
		}
	}
	ControllerType* againController;
	CTextEdit* textEdit;
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
