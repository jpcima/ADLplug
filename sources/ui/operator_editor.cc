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

#include "operator_editor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Operator_Editor::Operator_Editor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (component = new Styled_Knob_Default());
    component->setName ("new component");

    component->setBounds (16, 16, 48, 48);

    addAndMakeVisible (component2 = new Styled_Knob_Default());
    component2->setName ("new component");

    component2->setBounds (88, 16, 48, 48);

    addAndMakeVisible (component3 = new Styled_Knob_Default());
    component3->setName ("new component");

    component3->setBounds (160, 16, 48, 48);

    addAndMakeVisible (component4 = new Styled_Knob_Default());
    component4->setName ("new component");

    component4->setBounds (232, 16, 48, 48);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Operator_Editor::~Operator_Editor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;
    component2 = nullptr;
    component3 = nullptr;
    component4 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Operator_Editor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Operator_Editor::resized()
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

<JUCER_COMPONENT documentType="Component" className="Operator_Editor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="new component" id="7c54ff103d9f5d" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="16 16 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="be39ad00dcf6efe1" memberName="component2"
                    virtualName="" explicitFocusOrder="0" pos="88 16 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="8d88729c124c7b16" memberName="component3"
                    virtualName="" explicitFocusOrder="0" pos="160 16 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="7d576b68e9b588f" memberName="component4"
                    virtualName="" explicitFocusOrder="0" pos="232 16 48 48" class="Styled_Knob_Default"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
