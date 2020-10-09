include(AddVST3AuV3)

if(SMTG_MAC AND SMTG_ADD_VSTGUI)
    if(XCODE AND SMTG_IOS_DEVELOPMENT_TEAM)
        set(again_mac_app_sources
            "macOS/Sources/ViewController.m"
            "macOS/Sources/ViewController.h"
            "macOS/Sources/AppDelegate.m"
            "macOS/Sources/AppDelegate.h"
            "audiounitconfig.h"
        )

        set(again_ios_app_sources
            "iOS/Sources/ViewController.m"
            "iOS/Sources/ViewController.h"
            "iOS/Sources/AppDelegate.m"
            "iOS/Sources/AppDelegate.h"
            "iOS/Sources/main.mm"
            "audiounitconfig.h"
        )

        set(again_mac_app_ui_resources
            "macOS/Resources/Base.lproj/Main.storyboard"
            "macOS/Resources/again.icns"
            "Shared/drumLoop.wav"
        )

        set(again_ios_app_ui_resources
            "iOS/Resources/Base.lproj/LaunchScreen.storyboard"
            "iOS/Resources/Base.lproj/Main.storyboard"
            "iOS/Resources/Assets.xcassets"
            "Shared/drumLoop.wav"
        )

        #--------------------------------------------------------------------------------------------------------
        # macOS target
        #--------------------------------------------------------------------------------------------------------
        smtg_add_auv3(again_auv3_macos "macOS" "AGain AUV3 macOS" "com.steinberg.sdk.auv3.againmac" audiounitconfig.h "macOS/again.entitlements" "${again_mac_app_sources}" "${again_mac_app_ui_resources}" "macOS/Resources/Info.plist" "Shared/Info.plist" again)

        #--------------------------------------------------------------------------------------------------------
        # iOS target
        #--------------------------------------------------------------------------------------------------------
        smtg_add_auv3(again_auv3_ios "iOS" "AGain AUV3 iOS" "com.steinberg.sdk.auv3.againios" "audiounitconfig.h" "iOS/again.entitlements" "${again_ios_app_sources}" "${again_ios_app_ui_resources}" "iOS/Resources/Info.plist" "Shared/Info.plist" again_ios)

    endif()
endif()