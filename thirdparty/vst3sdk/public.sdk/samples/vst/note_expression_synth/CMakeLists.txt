
if(SMTG_ADD_VSTGUI)
    set(noteexpressionsynth_sources
        source/brownnoise.h
        source/factory.cpp
        source/filter.h
        source/note_expression_synth_controller.cpp
        source/note_expression_synth_controller.h
        source/note_expression_synth_processor.cpp
        source/note_expression_synth_processor.h
        source/note_expression_synth_ui.cpp
        source/note_expression_synth_ui.h
        source/note_expression_synth_voice.cpp
        source/note_expression_synth_voice.h
        source/note_touch_controller.cpp
        source/note_touch_controller.h
        source/version.h
        ${VSTGUI_ROOT}/vstgui4/vstgui/contrib/keyboardview.cpp
        ${VSTGUI_ROOT}/vstgui4/vstgui/contrib/keyboardview.h
        resource/note_expression_synth.uidesc
     )

    set(target noteexpressionsynth)

    smtg_add_vst3plugin(${target} ${noteexpressionsynth_sources})
    set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
    target_link_libraries(${target} PRIVATE base sdk vstgui_support)

    smtg_add_vst3_resource(${target} "resource/note_expression_synth.uidesc")
    smtg_add_vst3_resource(${target} "resource/about.png")
    smtg_add_vst3_resource(${target} "resource/background.png")
    smtg_add_vst3_resource(${target} "resource/groupframe.png")
    smtg_add_vst3_resource(${target} "resource/knob.png")
    smtg_add_vst3_resource(${target} "resource/knob2.png")
    smtg_add_vst3_resource(${target} "resource/knob big.png")
    smtg_add_vst3_resource(${target} "resource/vst3_logo_small.png")
    
    smtg_add_vst3_snapshot(${target} "resource/6EE65CD1B83A4AF480AA7929AEA6B8A0_snapshot.png")
    smtg_add_vst3_snapshot(${target} "resource/6EE65CD1B83A4AF480AA7929AEA6B8A0_snapshot_2.0x.png")

    if(SMTG_MAC)
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
    elseif(SMTG_WIN)
        target_sources(${target} PRIVATE resource/note_expression_synth.rc)
    endif()

    if(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
        set(target noteexpressionsynth_ios)
        smtg_add_ios_vst3plugin("${SMTG_CODE_SIGN_IDENTITY_IOS}" ${target} "${target}" "${noteexpressionsynth_sources}")

        set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
        target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
        target_link_libraries(${target} PRIVATE
            base_ios 
            sdk_ios
            "-framework UIKit"
            "-framework CoreGraphics" 
            "-framework QuartzCore" 
            "-framework CoreText" 
            "-framework Accelerate" 
            "-framework ImageIO" 
            "-framework MobileCoreServices" 
        )

        smtg_add_vst3_resource(${target} "resource/note_expression_synth.uidesc")
        smtg_add_vst3_resource(${target} "resource/about.png")
        smtg_add_vst3_resource(${target} "resource/background.png")
        smtg_add_vst3_resource(${target} "resource/groupframe.png")
        smtg_add_vst3_resource(${target} "resource/knob.png")
        smtg_add_vst3_resource(${target} "resource/knob2.png")
        smtg_add_vst3_resource(${target} "resource/knob big.png")
        smtg_add_vst3_resource(${target} "resource/vst3_logo_small.png")
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
        target_sources(${target} PRIVATE 
            "${VSTGUI_ROOT}/vstgui4/vstgui/vstgui_uidescription.cpp"
            "${VSTGUI_ROOT}/vstgui4/vstgui/vstgui_ios.mm"
            "${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3editor.cpp"
            "${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3groupcontroller.cpp"
            "${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3padcontroller.cpp"
            "${SDK_ROOT}/public.sdk/source/vst/vstguieditor.cpp"
        )
    endif()

endif(SMTG_ADD_VSTGUI)
