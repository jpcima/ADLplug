# JUCE library targets for ADLplug

set(JUCE_ROOT_DIR "${CMAKE_SOURCE_DIR}/thirdparty/JUCE")
include(JUCE)

add_juce_module(juce_core)
add_juce_module(juce_events)
add_juce_module(juce_data_structures)
add_juce_module(juce_audio_basics)
add_juce_module(juce_audio_devices)
add_juce_module(juce_audio_formats)
add_juce_module(juce_audio_processors)
add_juce_module(juce_audio_utils)
add_juce_module(juce_graphics)
add_juce_module(juce_gui_basics)
add_juce_module(juce_gui_extra)
target_link_libraries(juce_events PUBLIC juce_core)
target_link_libraries(juce_data_structures PUBLIC juce_events)
target_link_libraries(juce_audio_basics PUBLIC juce_core)
target_link_libraries(juce_audio_devices PUBLIC juce_audio_basics juce_events)
target_link_libraries(juce_audio_formats PUBLIC juce_audio_basics)
target_link_libraries(juce_audio_processors PUBLIC juce_gui_extra juce_audio_basics)
target_link_libraries(juce_audio_utils PUBLIC juce_gui_extra juce_audio_processors juce_audio_formats juce_audio_devices)
target_link_libraries(juce_gui_basics PUBLIC juce_graphics juce_data_structures)
target_link_libraries(juce_gui_extra PUBLIC juce_gui_basics)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # need this circular link dependency on Windows
  target_link_libraries(juce_events PUBLIC juce_gui_extra)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
else()
  # need this circular link dependency on X11
  target_link_libraries(juce_gui_basics PUBLIC juce_gui_extra)
endif()

if(ADLplug_VST2 AND NOT DEFINED ADLplug_VST2_SDK)
  if(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/thirdparty/vstsdk2.4")
    set(ADLplug_VST2_SDK "${PROJECT_SOURCE_DIR}/thirdparty/vstsdk2.4" CACHE STRING "VST2 SDK location")
  else()
    set(ADLplug_VST2_SDK "ADLplug_VST2_SDK-NOTFOUND" CACHE STRING "VST2 SDK location")
  endif()
  if(NOT ADLplug_VST2_SDK)
    message("VST2 SDK: VeSTige")
  else()
    message("VST2 SDK: Steinberg located at ${ADLplug_VST2_SDK}")
  endif()
endif()

if(ADLplug_VST2 AND ADLplug_VST2_SDK)
  target_compile_definitions(juce_audio_processors PUBLIC "JucePlugin_VST2SDK=1")
  target_include_directories(juce_audio_processors PUBLIC "${ADLplug_VST2_SDK}")
endif()

include(LinkHelpers)
target_link_static_threads(juce_core)

add_library(vst3sdk INTERFACE)
target_include_directories(vst3sdk INTERFACE "${PROJECT_SOURCE_DIR}/thirdparty/vst3sdk")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Windows
  target_link_libraries(juce_core PRIVATE winmm wininet ws2_32 shlwapi version)
  target_link_libraries(juce_gui_basics PRIVATE imm32)
  #
  set(ADLplug_ASIO_SDK "${PROJECT_SOURCE_DIR}/thirdparty/ASIOSDK2.3" CACHE STRING "ASIO SDK location")
  if(NOT EXISTS "${ADLplug_ASIO_SDK}/common/iasiodrv.h")
    message(WARNING "ASIO SDK not found in directory ${ADLplug_ASIO_SDK}. ASIO support disabled.")
    add_definitions("-DADLplug_ASIO=0")
  else()
    target_include_directories(juce_audio_devices PRIVATE "${ADLplug_ASIO_SDK}/common")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # Mac
  find_library(FOUNDATION_LIBRARY "Foundation")
  find_library(COCOA_LIBRARY "Cocoa")
  find_library(CARBON_LIBRARY "Carbon")
  find_library(CORE_SERVICES_LIBRARY "CoreServices")
  find_library(APPLICATION_SERVICES_LIBRARY "ApplicationServices")
  find_library(CORE_AUDIO_LIBRARY "CoreAudio")
  find_library(CORE_AUDIO_KIT_LIBRARY "CoreAudioKit")
  find_library(CORE_MIDI_LIBRARY "CoreMIDI")
  find_library(AUDIO_TOOLBOX_LIBRARY "AudioToolbox")
  find_library(ACCELERATE_LIBRARY "Accelerate")
  find_library(CORE_IMAGE_LIBRARY "CoreImage")
  find_library(IOKIT_LIBRARY "IOKit")
  find_library(AUDIO_UNIT_LIBRARY "AudioUnit")
  find_library(QUARTZ_CORE_LIBRARY "QuartzCore")
  target_link_libraries(juce_core PRIVATE "${FOUNDATION_LIBRARY}")
  target_link_libraries(juce_core PRIVATE "${COCOA_LIBRARY}")
  target_link_libraries(juce_core PRIVATE "${CARBON_LIBRARY}")
  target_link_libraries(juce_core PRIVATE "${CORE_SERVICES_LIBRARY}")
  target_link_libraries(juce_core PRIVATE "${APPLICATION_SERVICES_LIBRARY}")
  target_link_libraries(juce_audio_basics PRIVATE "${ACCELERATE_LIBRARY}")
  target_link_libraries(juce_audio_devices PRIVATE "${CORE_AUDIO_LIBRARY}")
  target_link_libraries(juce_audio_devices PRIVATE "${CORE_MIDI_LIBRARY}")
  target_link_libraries(juce_audio_formats PRIVATE "${AUDIO_TOOLBOX_LIBRARY}")
  target_link_libraries(juce_audio_utils PRIVATE "${CORE_AUDIO_KIT_LIBRARY}")
  target_link_libraries(juce_graphics PRIVATE "${CORE_IMAGE_LIBRARY}")
  target_link_libraries(juce_gui_basics PRIVATE "${IOKIT_LIBRARY}")
  target_link_libraries(juce_gui_basics PRIVATE "${QUARTZ_CORE_LIBRARY}")
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "^i.86$" OR
      CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
    find_library(CARBON_LIBRARY "Carbon")
    target_link_libraries(juce_core PRIVATE "${CARBON_LIBRARY}")
  endif()
else()
  # Linux and others
  include(FindPkgConfig)
  #
  target_link_libraries(juce_core PRIVATE dl)
  #
  find_package(ALSA REQUIRED)
  target_link_libraries(juce_audio_devices PRIVATE ${ALSA_LIBRARIES})
  target_include_directories(juce_audio_devices PRIVATE ${ALSA_INCLUDE_DIRS})
  #
  find_package(Freetype REQUIRED)
  target_link_libraries(juce_graphics PRIVATE ${FREETYPE_LIBRARIES})
  target_include_directories(juce_graphics PRIVATE ${FREETYPE_INCLUDE_DIRS})
  #
  find_package(X11 REQUIRED)
  target_link_libraries(juce_gui_basics PRIVATE ${X11_LIBRARIES})
  target_include_directories(juce_gui_basics PRIVATE ${X11_INCLUDE_DIRS})
  #
  if(FALSE)
    pkg_check_modules(GTK gtk+-3.0 REQUIRED)
    target_link_libraries(juce_gui_extra PRIVATE ${GTK_LIBRARIES})
    target_include_directories(juce_gui_extra PRIVATE ${GTK_INCLUDE_DIRS})
    link_directories(${GTK_LIBRARY_DIRS})
  endif()
endif()

if(ADLplug_VST2)
  set(VST2_SOURCES
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp")
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND VST2_SOURCES "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_VST_utils.mm")
  endif()
  add_juce_module(juce_audio_plugin_client_VST2 ${VST2_SOURCES})
  target_link_libraries(juce_audio_plugin_client_VST2 PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)
  if(ADLplug_VST2 AND ADLplug_VST2_SDK)
    target_compile_definitions(juce_audio_plugin_client_VST2 PUBLIC "JucePlugin_VST2SDK=1")
    target_include_directories(juce_audio_plugin_client_VST2 PUBLIC "${ADLplug_VST2_SDK}")
  endif()
endif()

if(ADLplug_VST3)
  set(VST3_SOURCES
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp")
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND VST3_SOURCES "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_VST_utils.mm")
  endif()
  add_juce_module(juce_audio_plugin_client_VST3 ${VST3_SOURCES})
  target_link_libraries(juce_audio_plugin_client_VST3 PUBLIC vst3sdk juce_gui_basics juce_audio_basics juce_audio_processors)
endif()

if(ADLplug_LV2)
  set(LV2_SOURCES
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_LV2.cpp")
  add_juce_module(juce_audio_plugin_client_LV2 ${LV2_SOURCES})
  target_link_libraries(juce_audio_plugin_client_LV2 PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)
endif()

if(ADLplug_AU)
  set(AU_SOURCES
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_AU_1.mm"
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_AU_2.mm")
  add_juce_module(juce_audio_plugin_client_AU ${AU_SOURCES})
  target_link_libraries(juce_audio_plugin_client_AU PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors "${QUARTZ_CORE_LIBRARY}" "${AUDIO_UNIT_LIBRARY}")
endif()

if(ADLplug_Standalone)
  set(Standalone_SOURCES
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp")
  add_juce_module(juce_audio_plugin_client_Standalone ${Standalone_SOURCES})
  target_link_libraries(juce_audio_plugin_client_Standalone PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)
endif()

if(ADLplug_Jack)
  set(StandaloneCustom_SOURCES
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${JUCE_PROJECT_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp")
  add_juce_module(juce_audio_plugin_client_StandaloneCustom ${StandaloneCustom_SOURCES})
  target_compile_definitions(juce_audio_plugin_client_StandaloneCustom PUBLIC "JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=1")
  target_link_libraries(juce_audio_plugin_client_StandaloneCustom PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)
endif()
