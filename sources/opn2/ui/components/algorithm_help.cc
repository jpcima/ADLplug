/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "algorithms.h"
#include "ui/components/algorithm_component.h"
//[/Headers]

#include "algorithm_help.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Algorithm_Help::Algorithm_Help ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    component.reset (new Algorithm_Component (Algorithms::algorithm_0));
    addAndMakeVisible (component.get());
    component->setName ("new component");

    component->setBounds (16, 32, 120, 152);

    component2.reset (new Algorithm_Component (Algorithms::algorithm_1));
    addAndMakeVisible (component2.get());
    component2->setName ("new component");

    component2->setBounds (168, 32, 120, 152);

    component3.reset (new Algorithm_Component (Algorithms::algorithm_4));
    addAndMakeVisible (component3.get());
    component3->setName ("new component");

    component3->setBounds (16, 216, 120, 90);

    component4.reset (new Algorithm_Component (Algorithms::algorithm_5));
    addAndMakeVisible (component4.get());
    component4->setName ("new component");

    component4->setBounds (168, 216, 120, 90);

    component5.reset (new Algorithm_Component (Algorithms::algorithm_6));
    addAndMakeVisible (component5.get());
    component5->setName ("new component");

    component5->setBounds (320, 216, 120, 90);

    component6.reset (new Algorithm_Component (Algorithms::algorithm_7));
    addAndMakeVisible (component6.get());
    component6->setName ("new component");

    component6->setBounds (472, 216, 160, 90);

    component7.reset (new Algorithm_Component (Algorithms::algorithm_2));
    addAndMakeVisible (component7.get());
    component7->setName ("new component");

    component7->setBounds (320, 32, 120, 152);

    component8.reset (new Algorithm_Component (Algorithms::algorithm_3));
    addAndMakeVisible (component8.get());
    component8->setName ("new component");

    component8->setBounds (472, 32, 120, 152);


    //[UserPreSize]
    double s = 18.0;
    component->scale(s);
    component2->scale(s);
    component3->scale(s);
    component4->scale(s);
    component5->scale(s);
    component6->scale(s);
    component7->scale(s);
    component8->scale(s);
    //[/UserPreSize]

    setSize (650, 318);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Algorithm_Help::~Algorithm_Help()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;
    component2 = nullptr;
    component3 = nullptr;
    component4 = nullptr;
    component5 = nullptr;
    component6 = nullptr;
    component7 = nullptr;
    component8 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Algorithm_Help::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    {
        int x = 16, y = 4, width = 120, height = 30;
        String text (TRANS("Algorithm 1"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 168, y = 4, width = 120, height = 30;
        String text (TRANS("Algorithm 2"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 16, y = 188, width = 120, height = 30;
        String text (TRANS("Algorithm 5"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 168, y = 188, width = 120, height = 30;
        String text (TRANS("Algorithm 6"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 320, y = 188, width = 120, height = 30;
        String text (TRANS("Algorithm 7"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 472, y = 188, width = 160, height = 30;
        String text (TRANS("Algorithm 8"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 320, y = 4, width = 120, height = 30;
        String text (TRANS("Algorithm 3"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 472, y = 4, width = 120, height = 30;
        String text (TRANS("Algorithm 4"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Algorithm_Help::resized()
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

<JUCER_COMPONENT documentType="Component" className="Algorithm_Help" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.33"
                 fixedSize="0" initialWidth="650" initialHeight="318">
  <BACKGROUND backgroundColour="efefef">
    <TEXT pos="16 4 120 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm 1"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="168 4 120 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm 2"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="16 188 120 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm 5"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="168 188 120 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm 6"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="320 188 120 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm 7"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="472 188 160 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm 8"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="320 4 120 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm 3"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="472 4 120 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm 4"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="37c053abaff9d7b0" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="16 32 120 152" class="Algorithm_Component"
                    params="Algorithms::algorithm_0"/>
  <GENERICCOMPONENT name="new component" id="7f9d025602565aac" memberName="component2"
                    virtualName="" explicitFocusOrder="0" pos="168 32 120 152" class="Algorithm_Component"
                    params="Algorithms::algorithm_1"/>
  <GENERICCOMPONENT name="new component" id="e4ade658ba500512" memberName="component3"
                    virtualName="" explicitFocusOrder="0" pos="16 216 120 90" class="Algorithm_Component"
                    params="Algorithms::algorithm_4"/>
  <GENERICCOMPONENT name="new component" id="b1c4bbd60b0c7e2" memberName="component4"
                    virtualName="" explicitFocusOrder="0" pos="168 216 120 90" class="Algorithm_Component"
                    params="Algorithms::algorithm_5"/>
  <GENERICCOMPONENT name="new component" id="1360d6b770abf706" memberName="component5"
                    virtualName="" explicitFocusOrder="0" pos="320 216 120 90" class="Algorithm_Component"
                    params="Algorithms::algorithm_6"/>
  <GENERICCOMPONENT name="new component" id="61b55ee2f50f9421" memberName="component6"
                    virtualName="" explicitFocusOrder="0" pos="472 216 160 90" class="Algorithm_Component"
                    params="Algorithms::algorithm_7"/>
  <GENERICCOMPONENT name="new component" id="f3c67542986f7697" memberName="component7"
                    virtualName="" explicitFocusOrder="0" pos="320 32 120 152" class="Algorithm_Component"
                    params="Algorithms::algorithm_2"/>
  <GENERICCOMPONENT name="new component" id="2113e5905c5277ed" memberName="component8"
                    virtualName="" explicitFocusOrder="0" pos="472 32 120 152" class="Algorithm_Component"
                    params="Algorithms::algorithm_3"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
