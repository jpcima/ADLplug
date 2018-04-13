cmake_minimum_required(VERSION 3.6)

project(ADLplug VERSION 1.0.0 LANGUAGES CXX)

option(ADLplug_VST2 "Build VST2" ON)
option(ADLplug_VST3 "Build VST3" AUTO)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include("${CMAKE_SOURCE_DIR}/JUCE-targets.cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if(ADLplug_VST3 STREQUAL "AUTO")
    set(ADLplug_VST3 "ON" CACHE STRING "" FORCE)
  endif()
else()
  if(ADLplug_VST3 STREQUAL "AUTO")
    set(ADLplug_VST3 "OFF" CACHE STRING "" FORCE)
  elseif(ADLplug_VST3)
    message(FATAL_ERROR "VST3 is not supported on ${CMAKE_SYSTEM_NAME}")
  endif()
endif()

message("Build VST2: ${ADLplug_VST2}")
message("Build VST3: ${ADLplug_VST3}")

add_library(ADLplug_core STATIC
  sources/plugin_editor.cpp
  sources/plugin_processor.cpp)
target_link_libraries(ADLplug_core
  PRIVATE juce_core juce_events juce_data_structures
  juce_audio_basics juce_audio_devices juce_audio_formats juce_audio_processors juce_audio_utils
  juce_graphics juce_gui_basics juce_gui_extra)

if(ADLplug_VST2)
  add_library(ADLplug_VST2 MODULE)
  target_link_libraries(ADLplug_VST2 PRIVATE ADLplug_core juce_audio_plugin_client_VST2)
  if(CMAKE_COMPILER_IS_GNUCXX)
    set_property(TARGET ADLplug_VST2 APPEND_STRING
      PROPERTY LINK_FLAGS " -Wl,-u,main,-u,VSTPluginMain")
  endif()
endif()

if(ADLplug_VST3)
  add_library(ADLplug_VST3 MODULE)
  target_link_libraries(ADLplug_VST3 PRIVATE ADLplug_core juce_audio_plugin_client_VST3)
  if(CMAKE_COMPILER_IS_GNUCXX)
    set_property(TARGET ADLplug_VST3 APPEND_STRING
      PROPERTY LINK_FLAGS " -Wl,-u,GetPluginFactory")
  endif()
endif()
