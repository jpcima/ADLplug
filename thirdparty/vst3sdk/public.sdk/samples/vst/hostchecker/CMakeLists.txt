
if(SMTG_ADD_VSTGUI)
    set(hostchecker_sources
        resource/hostchecker.uidesc
        source/cids.h
        source/editorsizecontroller.cpp
        source/editorsizecontroller.h
        source/eventlistcheck.cpp
        source/eventlistcheck.h
        source/eventlogdatabrowsersource.cpp
        source/eventlogdatabrowsersource.h
        source/eventlogger.cpp
        source/eventlogger.h
        source/factory.cpp
        source/hostcheck.cpp
        source/hostcheck.h
        source/hostcheckercontroller.cpp
        source/hostcheckercontroller.h
        source/hostcheckerprocessor.cpp
        source/hostcheckerprocessor.h
        source/logevents.h
        source/parameterchangescheck.cpp
        source/parameterchangescheck.h
        source/processcontextcheck.cpp
        source/processcontextcheck.h
        source/processsetupcheck.cpp
        source/processsetupcheck.h
        source/version.h
    )

    set(target hostchecker)
    smtg_add_vst3plugin(${target} ${hostchecker_sources})
    set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_include_directories(${target} PRIVATE ${VSTGUI_ROOT}/vstgui4)
    target_link_libraries(${target} PRIVATE base sdk vstgui_support)

    smtg_add_vst3_resource(${target} resource/hostchecker.uidesc)
    smtg_add_vst3_resource(${target} resource/bypass.png)
    smtg_add_vst3_resource(${target} resource/knob.png)

    if(SMTG_MAC)
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/info.plist" PREPROCESS)
    elseif(SMTG_WIN)
        target_sources(${target} PRIVATE resource/hostchecker.rc)
    endif()
endif(SMTG_ADD_VSTGUI)
