
set(syncdelay_sources
    source/sync.h
    source/syncdelaycontroller.cpp
    source/syncdelaycontroller.h
    source/syncdelayfactory.cpp
    source/syncdelayids.h
    source/syncdelayprocessor.cpp
    source/syncdelayprocessor.h
    source/syncdelayversion.h
)

set(target syncdelay)
smtg_add_vst3plugin(${target} ${syncdelay_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_PLUGIN_EXAMPLES_FOLDER})
target_link_libraries(${target} PRIVATE base sdk)
if(SMTG_MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(SMTG_WIN)
    target_sources(${target} PRIVATE resource/syncdelay.rc)
endif()
