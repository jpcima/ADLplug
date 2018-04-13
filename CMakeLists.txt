cmake_minimum_required(VERSION 3.6)

project(ADLplug VERSION 1.0.0 LANGUAGES CXX)
set(ADLplug_URI "https://github.com/jpcima/ADLplug")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(LinkHelpers)

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

message("Build VST2: ${ADLplug_VST2}")
message("Build VST3: ${ADLplug_VST3}")
message("Build LV2: ${ADLplug_LV2}")

add_library(ADLplug_core STATIC
  "sources/plugin_editor.cpp"
  "sources/plugin_processor.cpp")
target_link_libraries(ADLplug_core
  PRIVATE juce_core juce_events juce_data_structures
  juce_audio_basics juce_audio_devices juce_audio_formats juce_audio_processors juce_audio_utils
  juce_graphics juce_gui_basics juce_gui_extra)
set_property(TARGET ADLplug_core
  PROPERTY POSITION_INDEPENDENT_CODE ON)

macro(add_plugin NAME)
  add_library("${NAME}" MODULE ${ARGN})
  set_property(TARGET "${NAME}" PROPERTY PREFIX "")
endmacro()

if(ADLplug_VST2)
  add_plugin(ADLplug_VST2)
  target_link_libraries(ADLplug_VST2 PRIVATE juce_audio_plugin_client_VST2 ADLplug_core)
  require_symbols(ADLplug_VST2 "main" "VSTPluginMain")
endif()

if(ADLplug_VST3)
  add_plugin(ADLplug_VST3)
  target_link_libraries(ADLplug_VST3 PRIVATE juce_audio_plugin_client_VST3 ADLplug_core)
  require_symbols(ADLplug_VST3 "GetPluginFactory")
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
