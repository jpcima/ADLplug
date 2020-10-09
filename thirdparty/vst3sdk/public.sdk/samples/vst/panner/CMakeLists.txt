
if(SMTG_ADD_VSTGUI)
    set(plug_sources
        include/plugcontroller.h
        include/plugids.h
        include/plugprocessor.h
        include/version.h
        source/plugfactory.cpp
        source/plugcontroller.cpp
        source/plugprocessor.cpp
        resource/plug.uidesc
    )

    set(target panner)

    smtg_add_vst3plugin(${target} ${plug_sources})
    set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
    target_link_libraries(${target} PRIVATE base sdk vstgui_support)

    smtg_add_vst3_resource(${target} "resource/plug.uidesc")
    smtg_add_vst3_resource(${target} "resource/background.png")
    smtg_add_vst3_resource(${target} "resource/animation_knob.png")
    smtg_add_vst3_resource(${target} "resource/background_2x.png")
    smtg_add_vst3_resource(${target} "resource/background_3x.png")
    smtg_add_vst3_resource(${target} "resource/animation_knob_3x.png")

    smtg_add_vst3_snapshot(${target} "resource/A2EAF7DB320640F48EDE380DDF89562C_snapshot.png")
    smtg_add_vst3_snapshot(${target} "resource/A2EAF7DB320640F48EDE380DDF89562C_snapshot_2.0x.png")

    if(SMTG_MAC)
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
    elseif(SMTG_WIN)
        target_sources(${target} PRIVATE resource/plug.rc)
    endif()
endif(SMTG_ADD_VSTGUI)
