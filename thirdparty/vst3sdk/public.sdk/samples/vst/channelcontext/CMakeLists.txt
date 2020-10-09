
set(channelcontext_sources
    source/plug.cpp
    source/plug.h
    source/plugcids.h
    source/plugcontroller.cpp
    source/plugcontroller.h
    source/plugentry.cpp
    source/plugparamids.h
    source/version.h
)

set(target channelcontext)
smtg_add_vst3plugin(${target} ${channelcontext_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
target_link_libraries(${target} PRIVATE base sdk)
if(SMTG_MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(SMTG_WIN)
    target_sources(${target} PRIVATE resource/plug.rc)
endif()
