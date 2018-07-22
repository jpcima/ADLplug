#
# Options which take different default values depending on system
#
#
# Usage example:
#   system_option(QUUX ON "Windows" OFF "Darwin" OFF)
#

macro(system_option NAME DESCRIPTION VALUE)
  set(_args ${ARGN})
  set(_value "${VALUE}")
  while(_args)
    list(GET _args 0 _system)
    list(REMOVE_AT _args 0)
    if(CMAKE_SYSTEM_NAME STREQUAL "${_system}")
      list(GET _args 0 _value)
    endif()
    list(REMOVE_AT _args 0)
    unset(_system)
  endwhile()
  option("${NAME}" "${DESCRIPTION}" "${_value}")
  unset(_args)
  unset(_value)
endmacro()
