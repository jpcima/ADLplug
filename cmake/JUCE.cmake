# JUCE support stuff

if(NOT JUCE_ROOT_DIR)
  message(FATAL_ERROR "define JUCE_ROOT_DIR before including JUCE.cmake")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(JUCE_MODULE_SOURCE_SUFFIX ".mm")
else()
  set(JUCE_MODULE_SOURCE_SUFFIX ".cpp")
endif()

macro(add_juce_module NAME)
  set(_sources ${ARGN})
  if(NOT _sources)
    set(_sources "${PROJECT_SOURCE_DIR}/JuceLibraryCode/include_${NAME}${JUCE_MODULE_SOURCE_SUFFIX}")
  endif()
  add_library("${NAME}" STATIC "${_sources}")
  target_include_directories("${NAME}" PUBLIC "${JUCE_ROOT_DIR}/modules")
  set_property(TARGET "${NAME}" PROPERTY POSITION_INDEPENDENT_CODE ON)
  unset(_sources)
endmacro()
