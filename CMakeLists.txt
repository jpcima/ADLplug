cmake_minimum_required(VERSION 3.6)

project(ADLplug VERSION 1.0.0 LANGUAGES CXX)
set(ADLplug_URI "https://github.com/jpcima/ADLplug")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(LinkHelpers)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  if(CMAKE_COMPILER_IS_GNUCXX)
    # on MinGW JUCE defines a _WINNT_VER which disables some needed definitions
    add_definitions("-DREPLACEFILE_IGNORE_ACL_ERRORS=0x4")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

### ADLMIDI
set(WITH_MIDI_SEQUENCER OFF CACHE STRING "")
set(WITH_MUS_SUPPORT OFF CACHE STRING "")
set(WITH_XMI_SUPPORT OFF CACHE STRING "")
set(libADLMIDI_STATIC ON CACHE STRING "")
set(libADLMIDI_SHARED OFF CACHE STRING "")
add_subdirectory("thirdparty/libADLMIDI")

###
include("${CMAKE_SOURCE_DIR}/JUCE-targets.cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR
    CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if(NOT DEFINED ADLplug_VST2)
    set(ADLplug_VST2 "ON" CACHE STRING "" FORCE)
  endif()
  if(NOT DEFINED ADLplug_VST3)
    set(ADLplug_VST3 "ON" CACHE STRING "" FORCE)
  endif()
  if(NOT DEFINED ADLplug_LV2)
    set(ADLplug_LV2 "OFF" CACHE STRING "" FORCE)
  endif()
else()
  if(NOT DEFINED ADLplug_VST2)
    set(ADLplug_VST2 "ON" CACHE STRING "" FORCE)
  endif()
  if(NOT DEFINED ADLplug_VST3)
    set(ADLplug_VST3 "OFF" CACHE STRING "" FORCE)
  elseif(ADLplug_VST3)
    message(FATAL_ERROR "VST3 is not supported on ${CMAKE_SYSTEM_NAME}")
  endif()
  if(NOT DEFINED ADLplug_LV2)
    set(ADLplug_LV2 "ON" CACHE STRING "" FORCE)
  endif()
endif()
if(NOT DEFINED ADLplug_Standalone)
  set(ADLplug_Standalone "ON" CACHE STRING "" FORCE)
endif()

message("Build VST2: ${ADLplug_VST2}")
message("Build VST3: ${ADLplug_VST3}")
message("Build LV2: ${ADLplug_LV2}")

add_library(ADLplug_core STATIC
  "sources/plugin_editor.cc"
  "sources/plugin_processor.cc"
  "sources/adl/player.cc")
target_include_directories(ADLplug_core PRIVATE "sources")
target_link_libraries(ADLplug_core
  PRIVATE juce_core juce_events juce_data_structures
  juce_audio_basics juce_audio_devices juce_audio_formats juce_audio_processors juce_audio_utils
  juce_graphics juce_gui_basics juce_gui_extra
  PRIVATE ADLMIDI_static)
set_property(TARGET ADLplug_core
  PROPERTY POSITION_INDEPENDENT_CODE ON)

macro(add_plugin NAME)
  add_library("${NAME}" MODULE ${ARGN})
  set_property(TARGET "${NAME}" PROPERTY PREFIX "")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_COMPILER_IS_GNUCXX)
    # forces the static link of standard libraries
    set_property(TARGET "${NAME}" APPEND_STRING
      PROPERTY LINK_FLAGS " -static-libgcc -static-libstdc++")
    # forces the static link of winpthread
    set_property(TARGET "${NAME}" APPEND_STRING
      PROPERTY LINK_FLAGS " -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive")
  endif()
endmacro()

if(ADLplug_VST2)
  add_plugin(ADLplug_VST2)
  target_link_libraries(ADLplug_VST2 PRIVATE juce_audio_plugin_client_VST2 ADLplug_core)
  require_symbols(ADLplug_VST2 "VSTPluginMain")
endif()

if(ADLplug_VST3)
  add_plugin(ADLplug_VST3)
  target_link_libraries(ADLplug_VST3 PRIVATE juce_audio_plugin_client_VST3 ADLplug_core)
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    require_symbols(ADLplug_VST3 "GetPluginFactory")
  endif()
endif()

if(ADLplug_LV2)
  add_plugin(ADLplug_LV2)
  target_link_libraries(ADLplug_LV2 PRIVATE juce_audio_plugin_client_LV2 ADLplug_core)
  require_symbols(ADLplug_LV2 "lv2_generate_ttl" "lv2_descriptor" "lv2ui_descriptor")

  set(ADLplug_LV2_INSTALL_DIR "lib/lv2/ADLplug.git")
  install(TARGETS ADLplug_LV2 DESTINATION "${ADLplug_LV2_INSTALL_DIR}")
  install(FILES
    "resources/lv2/ADLplug_LV2.ttl"
    "resources/lv2/manifest.ttl"
    "resources/lv2/presets.ttl"
    DESTINATION "${ADLplug_LV2_INSTALL_DIR}")
endif()

if(ADLplug_Standalone)
  add_executable(ADLplug_Standalone sources/main.cc)
  target_link_libraries(ADLplug_Standalone PRIVATE juce_audio_plugin_client_Standalone ADLplug_core)
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    require_symbols(ADLplug_Standalone "_WinMain@16")
  else()
    require_symbols(ADLplug_Standalone "main")
  endif()
  set_property(TARGET ADLplug_Standalone PROPERTY OUTPUT_NAME "ADLplug")

  install(TARGETS ADLplug_Standalone DESTINATION "bin")
endif()
