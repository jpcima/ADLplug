
if(SMTG_MAC)
    option(SMTG_BUILD_INTERAPPAUDIO "Enable building the iOS InterAppAudio examples (deprecated)" OFF)


    if(XCODE AND SMTG_IOS_DEVELOPMENT_TEAM AND SMTG_BUILD_INTERAPPAUDIO)

        message("****************************************************************************************************************************************")
        message("***** The iOS InterAppAudio wrapper is deprecated and may be removed in the next SDK update. Please switch to AudioUnit V3 on iOS. *****")
        message("****************************************************************************************************************************************")

        set(target interappaudio)
        
        set(${target}_sources
            AudioIO.mm
            AudioIO.h
            HostApp.mm
            HostApp.h
            MidiIO.mm
            MidiIO.h
            PresetBrowserViewController.mm
            PresetBrowserViewController.h
            PresetManager.mm
            PresetManager.h
            PresetSaveViewController.mm
            PresetSaveViewController.h
            SettingsViewController.mm
            SettingsViewController.h
            VST3Editor.mm
            VST3Editor.h
            VST3Plugin.mm
            VST3Plugin.h
            VSTInterAppAudioAppDelegateBase.mm
            VSTInterAppAudioAppDelegateBase.h
        )
        add_library(${target} STATIC ${${target}_sources})
        smtg_set_platform_ios(${target})
        set_target_properties(${target} PROPERTIES ${SDK_IDE_LIBS_FOLDER})
        target_link_libraries(${target} PRIVATE sdk_ios "-framework CoreGraphics" "-framework UIKit" "-framework CoreMIDI" "-framework AudioToolbox" "-framework AVFoundation")
    endif()
endif()