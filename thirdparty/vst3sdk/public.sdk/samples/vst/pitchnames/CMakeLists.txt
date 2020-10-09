
if(SMTG_ADD_VSTGUI)
    set(pitchnames_sources
        source/factory.cpp
        source/pitchnames.cpp
        source/pitchnames.h
        source/pitchnamesdatabrowsersource.cpp
        source/pitchnamesdatabrowsersource.h
        source/version.h
    )

    set(target pitchnames)
    smtg_add_vst3plugin(${target} ${pitchnames_sources})
    set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_include_directories(${target} PRIVATE ${VSTGUI_ROOT}/vstgui4)
    target_link_libraries(${target} PRIVATE base sdk vstgui_support)

    smtg_add_vst3_resource(${target} "resource/pitchnames.uidesc")

    if(SMTG_MAC)
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
    elseif(SMTG_WIN)
        target_sources(${target} PRIVATE resource/pitchnames.rc)
    endif()
endif(SMTG_ADD_VSTGUI)
