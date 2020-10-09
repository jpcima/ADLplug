
# iOS target
if(SMTG_MAC AND SMTG_BUILD_INTERAPPAUDIO)
    if(XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
        set(target noteexpressionsynth_iaa_ios)
        set(${target}_xib_resources
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/InterAppAudio/Images.xcassets
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/InterAppAudio/LaunchScreen.storyboard
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/InterAppAudio/noteexpressionsynth_ios.entitlements
            ${SDK_ROOT}/public.sdk/source/vst/interappaudio/PresetBrowserView.xib
            ${SDK_ROOT}/public.sdk/source/vst/interappaudio/PresetSaveView.xib
            ${SDK_ROOT}/public.sdk/source/vst/interappaudio/SettingsView.xib
            InterAppAudioExample/VSTInterAppAudioHostUIControllerView.xib
        )
        set(${target}_sources
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/brownnoise.h
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/factory.cpp
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/filter.h
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_controller.cpp
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_controller.h
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_processor.cpp
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_processor.h
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_ui.cpp
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_ui.h
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_voice.cpp
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_voice.h
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_touch_controller.cpp
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/note_touch_controller.h
            ${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/source/version.h
            ${SDK_ROOT}/public.sdk/source/main/macmain.cpp
            ${SDK_ROOT}/public.sdk/source/vst/auwrapper/NSDataIBStream.mm
            ${SDK_ROOT}/public.sdk/source/vst/hosting/eventlist.cpp
            ${SDK_ROOT}/public.sdk/source/vst/hosting/hostclasses.cpp
            ${SDK_ROOT}/public.sdk/source/vst/hosting/parameterchanges.cpp
            ${SDK_ROOT}/public.sdk/source/vst/hosting/pluginterfacesupport.cpp
            ${SDK_ROOT}/public.sdk/source/vst/hosting/processdata.cpp
            ${SDK_ROOT}/public.sdk/source/vst/vstguieditor.cpp
            ${VSTGUI_ROOT}/vstgui4/vstgui/contrib/keyboardview.cpp
            ${VSTGUI_ROOT}/vstgui4/vstgui/contrib/keyboardview.h
            ${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3editor.cpp
            ${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3editor.h
            ${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3groupcontroller.cpp
            ${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3groupcontroller.h
            ${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3padcontroller.cpp
            ${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3padcontroller.h
            ${VSTGUI_ROOT}/vstgui4/vstgui/vstgui_ios.mm
            ${VSTGUI_ROOT}/vstgui4/vstgui/vstgui_uidescription.cpp
            InterAppAudioExample/VSTInterAppAudioAppDelegate.mm
            InterAppAudioExample/VSTInterAppAudioHostUIControllerViewController.mm
            InterAppAudioExample/main.m
        )
        set(prefix_header ${SDK_ROOT}/public.sdk/samples/vst/InterAppAudio/InterAppAudioExample/InterAppAudioExample-Prefix.pch)
        add_executable(${target} ${${target}_sources} ${${target}_xib_resources})
        smtg_set_platform_ios(${target})
        target_compile_options(${target} PUBLIC -DVSTGUI_LIVE_EDITING=0) # NO VSTGUI LIVE EDITING SUPPORT FOR iOS
        set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
        set_target_properties(${target} PROPERTIES BUNDLE TRUE)
        set_target_properties(${target} PROPERTIES RESOURCE "${${target}_xib_resources}")
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_LIST_DIR}/../note_expression_synth/resource/InterAppAudio/noteexpressionsynth_ios.entitlements")
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${SMTG_CODE_SIGN_IDENTITY_IOS}")
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "NO") # prevent warning
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${prefix_header})
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_GENERATE_PKGINFO_FILE "YES")
        target_link_libraries(${target} PRIVATE interappaudio "-framework QuartzCore" "-framework MobileCoreServices" "-framework Accelerate" "-framework ImageIO" "-framework GLKit" "-framework CoreText")
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/../note_expression_synth/resource/InterAppAudio/NoteExpressionSynthExample.plist" PREPROCESS)
        target_include_directories(${target} PUBLIC ${ROOT}/vstgui4)

        smtg_add_vst3_resource(${target} "${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/about.png")
        smtg_add_vst3_resource(${target} "${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/background.png")
        smtg_add_vst3_resource(${target} "${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/groupframe.png")
        smtg_add_vst3_resource(${target} "${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/knob big.png")
        smtg_add_vst3_resource(${target} "${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/knob.png")
        smtg_add_vst3_resource(${target} "${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/knob2.png")
        smtg_add_vst3_resource(${target} "${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/note_expression_synth.uidesc")
        smtg_add_vst3_resource(${target} "${SDK_ROOT}/public.sdk/samples/vst/note_expression_synth/resource/vst3_logo_small.png")

        # Development Team 
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${SMTG_IOS_DEVELOPMENT_TEAM})
    else()
        message("* To enable building the InterAppAudio NoteExpressionSynth example for iOS you need to set the SMTG_IOS_DEVELOPMENT_TEAM and use the Xcode generator")
    endif()
endif()
