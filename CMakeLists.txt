cmake_minimum_required(VERSION 3.6)

project(ADLplug VERSION 1.0.0 LANGUAGES CXX C)
set(ADLplug_URI "https://github.com/jpcima/ADLplug")

set(ADLplug_VST2 "" CACHE STRING "Build VST2")
set(ADLplug_VST3 "" CACHE STRING "Build VST3")
set(ADLplug_LV2 "" CACHE STRING "Build LV2")
set(ADLplug_Standalone "" CACHE STRING "Build Standalone")
set(ADLplug_Jack "" CACHE STRING "Build Jack")

option(ADLplug_RT_CHECKER "Enable a detector of RT programming errors" OFF)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(GNUWarnings)
include(LinkHelpers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")

enable_gnu_warning("all")
disable_gnu_warning("multichar")

if(ADLplug_RT_CHECKER)
  add_definitions("-DADLplug_RT_CHECKER=1")
  set(ADLplug_RT_CHECKER_LINK_FLAGS "-Wl,--wrap=malloc -Wl,--wrap=realloc -Wl,--wrap=free -Wl,--wrap=memalign")
endif()

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
add_subdirectory("thirdparty/libADLMIDI" EXCLUDE_FROM_ALL)

###
if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR
    CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if(ADLplug_VST2 STREQUAL "")
    set(ADLplug_VST2 "ON" CACHE STRING "Build VST2" FORCE)
  endif()
  if(ADLplug_VST3 STREQUAL "")
    set(ADLplug_VST3 "ON" CACHE STRING "Build VST3" FORCE)
  endif()
  if(ADLplug_LV2 STREQUAL "")
    set(ADLplug_LV2 "OFF" CACHE STRING "Build LV2" FORCE)
  endif()
else()
  if(ADLplug_VST2 STREQUAL "")
    set(ADLplug_VST2 "ON" CACHE STRING "Build VST2" FORCE)
  endif()
  if(ADLplug_VST3 STREQUAL "")
    set(ADLplug_VST3 "OFF" CACHE STRING "Build VST3" FORCE)
  elseif(ADLplug_VST3)
    message(FATAL_ERROR "VST3 is not supported on ${CMAKE_SYSTEM_NAME}")
  endif()
  if(ADLplug_LV2 STREQUAL "")
    set(ADLplug_LV2 "ON" CACHE STRING "Build LV2" FORCE)
  endif()
endif()
if(ADLplug_Standalone STREQUAL "")
  set(ADLplug_Standalone "ON" CACHE STRING "Build Standalone" FORCE)
endif()

if(ADLplug_Jack STREQUAL "")
  include(FindPkgConfig)
  pkg_check_modules(JACK jack)
  if(JACK_FOUND)
    set(ADLplug_Jack "ON" CACHE STRING "Build Jack" FORCE)
  else()
    set(ADLplug_Jack "OFF" CACHE STRING "Build Jack" FORCE)
  endif()
elseif(ADLplug_Jack)
  include(FindPkgConfig)
  pkg_check_modules(JACK jack REQUIRED)
endif()

message("Build VST2: ${ADLplug_VST2}")
message("Build VST3: ${ADLplug_VST3}")
message("Build LV2: ${ADLplug_LV2}")
message("Build Standalone: ${ADLplug_Standalone}")
message("Build Jack: ${ADLplug_Jack}")

include("${CMAKE_SOURCE_DIR}/JUCE-targets.cmake")

add_library(wopl STATIC "thirdparty/wopl/wopl/wopl_file.c")
target_include_directories(wopl PUBLIC "thirdparty/wopl")
set_property(TARGET wopl PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(ADLplug_core STATIC
  "JuceLibraryCode/BinaryData.cpp"
  "sources/utility/simple_fifo.cc"
  "sources/utility/rt_checker.cc"
  "sources/plugin_editor.cc"
  "sources/plugin_processor.cc"
  "sources/messages.cc"
  "sources/ui/chips/opl3_waves.cc"
  "sources/ui/look_and_feel.cc"
  "sources/ui/knobman_skin.cc"
  "sources/ui/knob_component.cc"
  "sources/ui/styled_knobs.cc"
  "sources/ui/vu_meter.cc"
  "sources/ui/operator_editor.cc"
  "sources/ui/wave_label.cc"
  "sources/ui/indicator_NxM.cc"
  "sources/ui/about_component.cc"
  "sources/ui/main_component.cc"
  "sources/ui/graphics_utils.cc"
  "sources/ui/image_utils.cc"
  "sources/adl/player.cc"
  "sources/adl/instrument.cc")
target_include_directories(ADLplug_core PRIVATE "sources")
target_link_libraries(ADLplug_core
  PUBLIC vf_freetype vf_gui
  juce_core juce_events juce_data_structures
  juce_audio_basics juce_audio_devices juce_audio_formats juce_audio_processors juce_audio_utils
  juce_graphics juce_gui_basics juce_gui_extra
  PRIVATE ADLMIDI_static wopl)
set_property(TARGET ADLplug_core
  PROPERTY POSITION_INDEPENDENT_CODE ON)

macro(add_plugin NAME)
  add_library("${NAME}" MODULE "sources/nothing.cc" ${ARGN})
  set_property(TARGET "${NAME}" PROPERTY PREFIX "")
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_COMPILER_IS_GNUCXX)
    # forces the static link of standard libraries
    set_property(TARGET "${NAME}" APPEND_STRING
      PROPERTY LINK_FLAGS " -static-libgcc -static-libstdc++")
    # forces the static link of winpthread
    set_property(TARGET "${NAME}" APPEND_STRING
      PROPERTY LINK_FLAGS " -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive")
  endif()
  if(ADLplug_RT_CHECKER)
    set_property(TARGET "${NAME}" APPEND_STRING
      PROPERTY LINK_FLAGS " ${ADLplug_RT_CHECKER_LINK_FLAGS}")
  endif()
endmacro()

if(ADLplug_VST2)
  add_plugin(ADLplug_VST2)
  target_link_libraries(ADLplug_VST2 PRIVATE juce_audio_plugin_client_VST2 ADLplug_core)
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(ADLplug_VST2 PRIVATE "${PROJECT_SOURCE_DIR}/sources/link/VST2.def")
  else()
    require_symbols(ADLplug_VST2 "VSTPluginMain")
  endif()
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(ADLplug_VST2 PROPERTIES
      PREFIX "" OUTPUT_NAME "ADLplug" SUFFIX ""
      LIBRARY_OUTPUT_DIRECTORY "vst2/ADLplug.vst/Contents/MacOS")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/vst2/ADLplug.vst/Contents/")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/vst2/ADLplug.vst/Contents/PkgInfo" "BNDL????")
    add_custom_target(ADLplug_VST2_InfoPlist ALL
      "${CMAKE_COMMAND}"
      "-DCONFIGURE_FILE_INPUT=${PROJECT_SOURCE_DIR}/juce/Builds/MacOSX/Info-VST.plist"
      "-DCONFIGURE_FILE_OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/vst2/ADLplug.vst/Contents/Info.plist"
      "-DEXECUTABLE_NAME=ADLplug"
      "-P" "${PROJECT_SOURCE_DIR}/cmake/ConfigureFile.cmake"
      DEPENDS "${PROJECT_SOURCE_DIR}/juce/Builds/MacOSX/Info-VST.plist")
  endif()
endif()

if(ADLplug_VST3)
  add_plugin(ADLplug_VST3)
  target_link_libraries(ADLplug_VST3 PRIVATE juce_audio_plugin_client_VST3 ADLplug_core)
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(ADLplug_VST3 PRIVATE "${PROJECT_SOURCE_DIR}/sources/link/VST3.def")
  else()
    require_symbols(ADLplug_VST3 "GetPluginFactory")
  endif()
endif()

if(ADLplug_LV2)
  add_plugin(ADLplug_LV2)
  target_link_libraries(ADLplug_LV2 PRIVATE juce_audio_plugin_client_LV2 ADLplug_core)
  require_symbols(ADLplug_LV2 "lv2_generate_ttl" "lv2_descriptor" "lv2ui_descriptor")
  set_property(TARGET ADLplug_LV2
    PROPERTY LIBRARY_OUTPUT_DIRECTORY "lv2/ADLplug.lv2")

  set(ADLplug_LV2_MANIFESTS
    "resources/lv2/ADLplug_LV2.ttl"
    "resources/lv2/manifest.ttl"
    "resources/lv2/presets.ttl")
  add_custom_target(ADLplug_LV2_manifests_copy ALL
    COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${CMAKE_CURRENT_BINARY_DIR}/lv2/ADLplug.lv2/"
    COMMAND "${CMAKE_COMMAND}" "-E" "copy" ${ADLplug_LV2_MANIFESTS} "${CMAKE_CURRENT_BINARY_DIR}/lv2/ADLplug.lv2/"
    DEPENDS ${ADLplug_LV2_MANIFESTS}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

  set(ADLplug_LV2_INSTALL_DIR "lib/lv2/ADLplug.lv2")
  install(TARGETS ADLplug_LV2 DESTINATION "${ADLplug_LV2_INSTALL_DIR}")
  install(FILES ${ADLplug_LV2_MANIFESTS} DESTINATION "${ADLplug_LV2_INSTALL_DIR}")
endif()

if(ADLplug_Standalone)
  add_executable(ADLplug_Standalone "sources/nothing.cc")
  target_link_libraries(ADLplug_Standalone PRIVATE juce_audio_plugin_client_Standalone ADLplug_core)
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    require_symbols(ADLplug_Standalone "_WinMain@16")
  else()
    require_symbols(ADLplug_Standalone "main")
  endif()
  set_property(TARGET ADLplug_Standalone PROPERTY OUTPUT_NAME "ADLplug")
  if(ADLplug_RT_CHECKER)
    set_property(TARGET ADLplug_Standalone APPEND_STRING
      PROPERTY LINK_FLAGS " ${ADLplug_RT_CHECKER_LINK_FLAGS}")
  endif()
  install(TARGETS ADLplug_Standalone DESTINATION "bin")
endif()

if(ADLplug_Jack)
  add_executable(ADLplug_Jack "sources/jack_main.cc")
  target_link_libraries(ADLplug_Jack PRIVATE juce_audio_plugin_client_StandaloneCustom ADLplug_core ${JACK_LIBRARIES})
  target_include_directories(ADLplug_Jack PRIVATE ${JACK_INCLUDE_DIRS})
  link_directories(${JACK_LIBRARY_DIRS})
  set_property(TARGET ADLplug_Jack PROPERTY OUTPUT_NAME "ADLplug_jack")
  if(ADLplug_RT_CHECKER)
    set_property(TARGET ADLplug_Jack APPEND_STRING
      PROPERTY LINK_FLAGS " ${ADLplug_RT_CHECKER_LINK_FLAGS}")
  endif()
  install(TARGETS ADLplug_Jack DESTINATION "bin")
endif()
