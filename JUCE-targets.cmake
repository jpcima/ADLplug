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

find_package(Threads REQUIRED)
target_link_libraries(juce_core PRIVATE ${CMAKE_THREAD_LIBS_INIT})

add_library(vst3sdk INTERFACE)
target_include_directories(vst3sdk INTERFACE "${PROJECT_SOURCE_DIR}/thirdparty/vst3sdk")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Windows
  # TODO

  target_link_libraries(juce_core PRIVATE winmm wininet ws2_32 shlwapi version)
  target_link_libraries(juce_gui_basics PRIVATE imm32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # Mac
  # TODO
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
  target_link_libraries(juce_graphics PRIVATE Freetype::Freetype)
  #
  find_package(X11 REQUIRED)
  target_link_libraries(juce_gui_basics PRIVATE ${X11_LIBRARIES})
  target_include_directories(juce_gui_basics PRIVATE ${X11_INCLUDE_DIRS})
  #
  pkg_check_modules(GTK gtk+-3.0 REQUIRED)
  target_link_libraries(juce_gui_extra PRIVATE ${GTK_LIBRARIES})
  target_include_directories(juce_gui_extra PRIVATE ${GTK_INCLUDE_DIRS})
  link_directories(${GTK_LIBRARY_DIRS})
endif()

if(ADLplug_VST2)
  set(VST2_SOURCES
    "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp")
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND VST2_SOURCES "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_VST_utils.mm")
  endif()
  add_juce_module(juce_audio_plugin_client_VST2 ${VST2_SOURCES})
  target_link_libraries(juce_audio_plugin_client_VST2 PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)
endif()

if(ADLplug_VST3)
  set(VST3_SOURCES
    "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp")
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND VST3_SOURCES "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_VST_utils.mm")
  endif()
  add_juce_module(juce_audio_plugin_client_VST3 ${VST3_SOURCES})
  target_link_libraries(juce_audio_plugin_client_VST3 PUBLIC vst3sdk juce_gui_basics juce_audio_basics juce_audio_processors)
endif()

if(ADLplug_LV2)
  set(LV2_SOURCES
    "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_LV2.cpp")
  add_juce_module(juce_audio_plugin_client_LV2 ${LV2_SOURCES})
  target_link_libraries(juce_audio_plugin_client_LV2 PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)

  target_compile_definitions(juce_audio_plugin_client_LV2
    PUBLIC "JucePlugin_Build_LV2=1"
    PUBLIC "JucePlugin_LV2URI=\"${ADLplug_URI}\"")
endif()

if(ADLplug_Standalone)
  set(Standalone_SOURCES
    "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp"
    "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp")
  add_juce_module(juce_audio_plugin_client_Standalone ${Standalone_SOURCES})
  target_link_libraries(juce_audio_plugin_client_Standalone PUBLIC juce_gui_basics juce_audio_basics juce_audio_processors)
endif()
