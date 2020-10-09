
cmake_minimum_required(VERSION 3.4.3)

project(smtg_public_sdk)

add_library(sdk STATIC 
    source/common/commoniids.cpp
    source/common/openurl.cpp
    source/common/openurl.h
    source/common/pluginview.cpp
    source/common/pluginview.h
    source/common/threadchecker_linux.cpp
    source/common/threadchecker_win32.cpp
    source/common/threadchecker.h
    source/main/pluginfactory.cpp
    source/main/pluginfactory.h
)
if(SMTG_MAC)
    target_sources(sdk PRIVATE
        source/common/threadchecker_mac.mm
    )
endif()

if(NOT VST_SDK)
    target_sources(sdk PRIVATE
        source/common/pvaluecontainer.cpp
        source/common/pvaluecontainer.h
    )
endif()

# Additional sources for VST3 SDK usage
if(VST_SDK)
    target_sources(sdk PRIVATE
        source/vst/utility/processcontextrequirements.h
        source/vst/utility/ringbuffer.h
        source/vst/utility/vst2persistence.h
        source/vst/utility/vst2persistence.cpp
        source/vst/vstaudioeffect.cpp
        source/vst/vstaudioeffect.h
        source/vst/vstbus.cpp
        source/vst/vstbus.h
        source/vst/vstbypassprocessor.h
        source/vst/vstcomponent.cpp
        source/vst/vstcomponent.h
        source/vst/vstcomponentbase.cpp
        source/vst/vstcomponentbase.h
        source/vst/vsteditcontroller.cpp
        source/vst/vsteditcontroller.h
        source/vst/vsteventshelper.h
        source/vst/vsthelpers.h
        source/vst/vstinitiids.cpp
        source/vst/vstnoteexpressiontypes.cpp
        source/vst/vstnoteexpressiontypes.h
        source/vst/vstparameters.cpp
        source/vst/vstparameters.h
        source/vst/vstpresetfile.cpp
        source/vst/vstpresetfile.h
        source/vst/vstrepresentation.cpp
        source/vst/vstrepresentation.h
    )
endif()

target_link_libraries(sdk PRIVATE 
    base
    pluginterfaces
)

smtg_setup_universal_binary(sdk)

get_target_property(sdk_SOURCES sdk SOURCES)
source_group("public.sdk"   FILES ${sdk_SOURCES})

# iOS target
if(SMTG_MAC AND XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
    add_library(sdk_ios STATIC 
        ${sdk_SOURCES}
    )
    smtg_set_platform_ios(sdk_ios)
    set_target_properties(sdk_ios PROPERTIES ${SDK_IDE_LIBS_FOLDER})
    target_link_libraries(sdk_ios PRIVATE 
        base_ios
        pluginterfaces_ios
    )
endif()
