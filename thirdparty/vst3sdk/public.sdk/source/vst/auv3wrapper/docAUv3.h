//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/auv3wrapper/docAUv3.h
// Created by  : Steinberg, 07/2017.
// Description : VST 3 AUv3Wrapper
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

/**
*******************************************
\page AUv3Wrapper VST 3 - Audio Unit v3 Wrapper
*******************************************
\tableofcontents

\brief
Helper Class wrapping a VST 3 plug-in to a Audio Unit v3 plug-in

***************************
\section AUv3Introduction Introduction
***************************
The VST 3 SDK comes with a helper class which wraps one VST 3 Audio Processor and Edit Controller to
a AUv3 plug-in.

The wrapped AudioUnit does support MPE when the VST3 plug-in has Note Expression support. You need
to implement \ref Steinberg::Vst::INoteExpressionPhysicalUIMapping to map your Note Expression to
the limited three expressions defined by MPE.
\n

***************************
\section howtoAUv3 How does it work?
***************************
- Structure:
    - App          (container app which initializes the AU through small Playback Engine)
    - Extension    (extension which is loaded by hosts, also initializes the AU)

- How-To use the VST3->AUv3 Wrapper with the sample code:

    - make sure you have correct code signing set up

    - build & run targets

- How-To use the VST3->AUv3 Wrapper with your own VST Plugin:

    - duplicate either the folder again_auv3 or note_expression_synth_auv3 in public.sdk/samples/vst,
      rename it as you like and edit the CMakelist.txt to add your sources, resources and modify target
      names etc.

    - take special attention to the stuff in audiounitconfig.h and change the definitions in
      there.

    - also change the other files in that folder to your needs. -Code signing is required, but you
      can build and test with a developer only identity.

    - build & run targets
*/
