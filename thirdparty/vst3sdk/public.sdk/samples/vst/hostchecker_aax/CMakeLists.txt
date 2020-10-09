
if(SMTG_AAX_SDK_PATH AND SMTG_ADD_VSTGUI)
    include(AddAAXLibrary)

    set(hostchecker_sources
        ../hostchecker/resource/hostchecker.uidesc
        ../hostchecker/source/cids.h
        ../hostchecker/source/editorsizecontroller.cpp
        ../hostchecker/source/editorsizecontroller.h
        ../hostchecker/source/eventlistcheck.cpp
        ../hostchecker/source/eventlistcheck.h
        ../hostchecker/source/eventlogdatabrowsersource.cpp
        ../hostchecker/source/eventlogdatabrowsersource.h
        ../hostchecker/source/eventlogger.cpp
        ../hostchecker/source/eventlogger.h
        ../hostchecker/source/factory.cpp
        ../hostchecker/source/hostcheck.cpp
        ../hostchecker/source/hostcheck.h
        ../hostchecker/source/hostcheckercontroller.cpp
        ../hostchecker/source/hostcheckercontroller.h
        ../hostchecker/source/hostcheckerprocessor.cpp
        ../hostchecker/source/hostcheckerprocessor.h
        ../hostchecker/source/logevents.h
        ../hostchecker/source/parameterchangescheck.cpp
        ../hostchecker/source/parameterchangescheck.h
        ../hostchecker/source/processcontextcheck.cpp
        ../hostchecker/source/processcontextcheck.h
        ../hostchecker/source/processsetupcheck.cpp
        ../hostchecker/source/processsetupcheck.h
        ../hostchecker/source/version.h
        source/hostchecker_aax.cpp
    )

    set(target hostchecker_aax)
    smtg_add_aaxplugin(${target} ${hostchecker_sources})
    set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
    target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
    target_include_directories(${target} PUBLIC ${SDK_ROOT}/public.sdk/samples/vst/hostchecker/source)
    target_link_libraries(${target} PRIVATE base sdk vstgui_support aaxwrapper)

    smtg_add_vst3_resource(${target} ../hostchecker/resource/hostchecker.uidesc)
    smtg_add_vst3_resource(${target} ../hostchecker/resource/bypass.png)
    smtg_add_vst3_resource(${target} ../hostchecker/resource/knob.png)

    if(SMTG_MAC)
        smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/../hostchecker/resource/info.plist" PREPROCESS)
    elseif(SMTG_WIN)
        target_sources(${target} PRIVATE ../hostchecker/resource/hostchecker.rc)
    endif()
endif()
