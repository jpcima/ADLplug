
if(SMTG_AAX_SDK_PATH AND SMTG_ADD_VSTGUI)
    include(AddAAXLibrary)

    set(again_sources
        source/againaax.cpp
        ../again/source/again.cpp
        ../again/source/again.h
        ../again/source/againcids.h
        ../again/source/againcontroller.cpp
        ../again/source/againcontroller.h
        ../again/source/againentry.cpp
        ../again/source/againparamids.h
        ../again/source/againprocess.h
        ../again/source/againsidechain.cpp
        ../again/source/againsidechain.h
        ../again/source/againuimessagecontroller.h
        ../again/source/version.h
    )

    set(target again_aax)
    smtg_add_aaxplugin(${target} ${again_sources})
    set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
    target_include_directories(${target} PUBLIC ${SDK_ROOT}/public.sdk/samples/vst/again/source)
    target_link_libraries(${target} PRIVATE base sdk vstgui_support aaxwrapper)

    smtg_add_vst3_resource(${target} "../again/resource/again.uidesc")
    smtg_add_vst3_resource(${target} "../again/resource/background.png")
    smtg_add_vst3_resource(${target} "../again/resource/slider_background.png")
    smtg_add_vst3_resource(${target} "../again/resource/slider_handle.png")
    smtg_add_vst3_resource(${target} "../again/resource/slider_handle_2.0x.png")
    smtg_add_vst3_resource(${target} "../again/resource/vu_on.png")
    smtg_add_vst3_resource(${target} "../again/resource/vu_off.png")

    if(SMTG_MAC)
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/../again/mac/Info.plist" PREPROCESS)
    elseif(SMTG_WIN)
        target_sources(${target} PRIVATE ../again/resource/again.rc)
        # remove warnings from VST2
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()

endif()
