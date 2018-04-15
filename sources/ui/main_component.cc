/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "main_component.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Main_Component::Main_Component ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (component = new Operator_Editor());
    component->setName ("new component");

    component->setBounds (16, 104, 248, 216);

    addAndMakeVisible (component2 = new Operator_Editor());
    component2->setName ("new component");

    component2->setBounds (288, 104, 248, 216);

    addAndMakeVisible (component3 = new Operator_Editor());
    component3->setName ("new component");

    component3->setBounds (19, 336, 248, 216);

    addAndMakeVisible (component4 = new Operator_Editor());
    component4->setName ("new component");

    component4->setBounds (291, 336, 248, 216);

    drawable1 = Drawable::createFromImageData (BinaryData::logo_png, BinaryData::logo_pngSize);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Main_Component::~Main_Component()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;
    component2 = nullptr;
    component3 = nullptr;
    component4 = nullptr;
    drawable1 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Main_Component::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    {
        int x = 20, y = 12, width = 308, height = 68;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable1 != 0);
        if (drawable1 != 0)
            drawable1->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.000f);
    }

    {
        int x = 356, y = 28, width = 284, height = 30;
        String text (TRANS("FM synthesizer with YMF262 chip emulation"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Main_Component::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Main_Component" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ff323e44">
    <IMAGE pos="20 12 308 68" resource="BinaryData::logo_png" opacity="1.00000000000000000000"
           mode="2"/>
    <TEXT pos="356 28 284 30" fill="solid: fff0f8ff" hasStroke="0" text="FM synthesizer with YMF262 chip emulation"
          fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
          bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="423f2b5d9aff978c" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="16 104 248 216" class="Operator_Editor"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="f946a77416e2893a" memberName="component2"
                    virtualName="" explicitFocusOrder="0" pos="288 104 248 216" class="Operator_Editor"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="3567a5024da26800" memberName="component3"
                    virtualName="" explicitFocusOrder="0" pos="19 336 248 216" class="Operator_Editor"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="40000b4e48953c47" memberName="component4"
                    virtualName="" explicitFocusOrder="0" pos="291 336 248 216" class="Operator_Editor"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
