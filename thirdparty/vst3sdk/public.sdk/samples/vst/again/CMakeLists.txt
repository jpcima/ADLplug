
if(SMTG_ADD_VSTGUI)
    if (SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)
        set(again_vst2_sources
            ${SDK_ROOT}/public.sdk/source/vst/vst2wrapper/vst2wrapper.sdk.cpp
            source/againentry_vst2.cpp
        )
    endif()

    set(again_sources
        source/again.cpp
        source/again.h
        source/againcids.h
        source/againcontroller.cpp
        source/againcontroller.h
        source/againentry.cpp
        source/againparamids.h
        source/againprocess.h
        source/againsidechain.cpp
        source/againsidechain.h
        source/againuimessagecontroller.h
        source/version.h
        resource/again.uidesc
    )

    set(again_simple_sources
        ${SDK_ROOT}/public.sdk/source/vst/vstsinglecomponenteffect.cpp
        ${SDK_ROOT}/public.sdk/source/vst/vstsinglecomponenteffect.h
        source/againparamids.h
        source/againsimple.cpp
        source/againsimple.h
        source/version.h
    )

    # VST2 is only defined for macOS and Windows
    if(SMTG_MAC OR SMTG_WIN)
        if (SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)
            set(again_sources ${again_sources} ${again_vst2_sources})
        else()
            set(again_sources ${again_sources})
        endif()
    endif()

    set(target again)
    smtg_add_vst3plugin(${target} ${again_sources})
    #smtg_add_vst3plugin(${target} PACKAGE_NAME "A Gain" SOURCES_LIST ${again_sources})
    set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
    target_link_libraries(${target} PRIVATE base sdk vstgui_support)

    smtg_add_vst3_resource(${target} "resource/again.uidesc")
    smtg_add_vst3_resource(${target} "resource/background.png")
    smtg_add_vst3_resource(${target} "resource/slider_background.png")
    smtg_add_vst3_resource(${target} "resource/slider_handle.png")
    smtg_add_vst3_resource(${target} "resource/slider_handle_2.0x.png")
    smtg_add_vst3_resource(${target} "resource/vu_on.png")
    smtg_add_vst3_resource(${target} "resource/vu_off.png")

    smtg_add_vst3_snapshot(${target} "resource/84E8DE5F92554F5396FAE4133C935A18_snapshot.png")
    smtg_add_vst3_snapshot(${target} "resource/84E8DE5F92554F5396FAE4133C935A18_snapshot_2.0x.png")

    if(SMTG_MAC)
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist" PREPROCESS)
    elseif(SMTG_WIN)
        target_sources(${target} PRIVATE resource/again.rc)
    endif()

    if(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
        set(target again_ios)
        smtg_add_ios_vst3plugin("${SMTG_CODE_SIGN_IDENTITY_IOS}" ${target} "${target}" "${again_sources}")
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

        smtg_add_vst3_resource(${target} "resource/again.uidesc")
        smtg_add_vst3_resource(${target} "resource/background.png")
        smtg_add_vst3_resource(${target} "resource/slider_background.png")
        smtg_add_vst3_resource(${target} "resource/slider_handle.png")
        smtg_add_vst3_resource(${target} "resource/slider_handle_2.0x.png")
        smtg_add_vst3_resource(${target} "resource/vu_on.png")
        smtg_add_vst3_resource(${target} "resource/vu_off.png")

        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist" PREPROCESS)
        target_sources(${target} PRIVATE 
            ${VSTGUI_ROOT}/vstgui4/vstgui/vstgui_uidescription.cpp
            ${VSTGUI_ROOT}/vstgui4/vstgui/vstgui_ios.mm
            ${VSTGUI_ROOT}/vstgui4/vstgui/plugin-bindings/vst3editor.cpp
            ${SDK_ROOT}/public.sdk/source/vst/vstguieditor.cpp
        )
    endif()

    set(targetsimple againsimple)
    smtg_add_vst3plugin(${targetsimple} ${again_simple_sources})
    set_target_properties(${targetsimple} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_include_directories(${targetsimple} PUBLIC ${VSTGUI_ROOT}/vstgui4)
    target_link_libraries(${targetsimple} PRIVATE base sdk vstgui_support)

    smtg_add_vst3_resource(${targetsimple} "resource/again.uidesc")
    smtg_add_vst3_resource(${targetsimple} "resource/background.png")
    smtg_add_vst3_resource(${targetsimple} "resource/slider_background.png")
    smtg_add_vst3_resource(${targetsimple} "resource/slider_handle.png")
    smtg_add_vst3_resource(${targetsimple} "resource/slider_handle_2.0x.png")
    smtg_add_vst3_resource(${targetsimple} "resource/vu_on.png")
    smtg_add_vst3_resource(${targetsimple} "resource/vu_off.png")

    if(SMTG_MAC)
        smtg_set_bundle(${targetsimple} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist" PREPROCESS)
    elseif(SMTG_WIN)
        target_sources(${targetsimple} PRIVATE resource/again.rc)
    endif()

    if (SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION)
        message(STATUS "SMTG_CREATE_VST2_AGAIN_SAMPLE_VERSION is set. A VST 2 version of the plug-in will be created (just rename the generated file from .vst3 to .dll (win) or .vst (mac)).")
        if(XCODE)
            # fix missing VSTPluginMain symbol when also building VST 2 version
            set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_EXPORTED_SYMBOLS_FILE "")
        endif()
        if (SMTG_WIN)
            add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        endif()
    endif()
endif(SMTG_ADD_VSTGUI)