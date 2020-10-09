
if(SMTG_ADD_VST3_HOSTING_SAMPLES)

    set(editorhost_sources
        ${SDK_ROOT}/public.sdk/source/vst/hosting/connectionproxy.cpp
        ${SDK_ROOT}/public.sdk/source/vst/hosting/connectionproxy.h
        ${SDK_ROOT}/public.sdk/source/vst/hosting/hostclasses.cpp
        ${SDK_ROOT}/public.sdk/source/vst/hosting/hostclasses.h
        ${SDK_ROOT}/public.sdk/source/vst/hosting/module.cpp
        ${SDK_ROOT}/public.sdk/source/vst/hosting/module.h
        ${SDK_ROOT}/public.sdk/source/vst/hosting/pluginterfacesupport.cpp
        ${SDK_ROOT}/public.sdk/source/vst/hosting/pluginterfacesupport.h
        ${SDK_ROOT}/public.sdk/source/vst/hosting/plugprovider.cpp
        ${SDK_ROOT}/public.sdk/source/vst/hosting/plugprovider.h
        ${SDK_ROOT}/public.sdk/source/vst/hosting/processdata.cpp
        ${SDK_ROOT}/public.sdk/source/vst/hosting/processdata.h
        ${SDK_ROOT}/public.sdk/source/vst/utility/optional.h
        ${SDK_ROOT}/public.sdk/source/vst/utility/stringconvert.cpp
        ${SDK_ROOT}/public.sdk/source/vst/utility/stringconvert.h
        ${SDK_ROOT}/public.sdk/source/vst/utility/uid.h
        ${SDK_ROOT}/public.sdk/source/vst/vstinitiids.cpp
        source/editorhost.cpp
        source/editorhost.h
        source/platform/appinit.h
        source/platform/iapplication.h
        source/platform/iplatform.h
        source/platform/iwindow.h
        source/usediids.cpp
    )

    if(SMTG_MAC)
        set(editorhost_sources
            ${editorhost_sources}
            source/platform/mac/platform.mm
            source/platform/mac/window.h
            source/platform/mac/window.mm
            ${SDK_ROOT}/public.sdk/source/vst/hosting/module_mac.mm
        )
        set(editorhost_PLATFORM_LIBS
            "-framework Cocoa"
        )
        get_filename_component(InfoPlistFile "resource/Info.plist" ABSOLUTE)
        set(APP_PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST ${InfoPlistFile}
        )
    elseif(SMTG_LINUX)
        find_package(X11 REQUIRED)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
        pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)
        include_directories(${X11_INCLUDE_DIR} ${GTKMM3_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})
        set(editorhost_sources
            ${editorhost_sources}
            source/platform/linux/platform.cpp
            source/platform/linux/runloop.h
            source/platform/linux/runloop.cpp
            source/platform/linux/window.h
            source/platform/linux/window.cpp
            ${SDK_ROOT}/public.sdk/source/vst/hosting/module_linux.cpp
        )
        set(editorhost_PLATFORM_LIBS
            ${X11_LIBRARIES}
            ${GTK3_LIBRARIES}
            ${GTKMM3_LIBRARIES}
        )
        set(APP_PROPERTIES
        )
    elseif(SMTG_WIN)
        set(editorhost_sources
            ${editorhost_sources}
            source/platform/win32/platform.cpp
            source/platform/win32/window.h
            source/platform/win32/window.cpp
            ${SDK_ROOT}/public.sdk/source/vst/hosting/module_win32.cpp
        )
        set(editorhost_PLATFORM_LIBS
        )
        set(APP_PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:windows"
        )
    endif()

    set(target editorhost)
    add_executable(${target} ${editorhost_sources})
    set_target_properties(${target} PROPERTIES ${SDK_IDE_HOSTING_EXAMPLES_FOLDER})
    target_link_libraries(${target} PRIVATE base sdk ${editorhost_PLATFORM_LIBS})
    smtg_setup_universal_binary(${target})
    if (APP_PROPERTIES)
        set_target_properties(${target} PROPERTIES ${APP_PROPERTIES})
    endif()

endif()
