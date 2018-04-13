# Link helpers

macro(require_symbols TARGET)
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND NOT MINGW)
    message(WARNING "TODO: implement Windows linker flags")
  else()
    foreach(_symbol ${ARGN})
      if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(_symbol "_${_symbol}")
      endif()
      set_property(TARGET "${TARGET}" APPEND_STRING
        PROPERTY LINK_FLAGS " -Wl,-u,${_symbol}")
    endforeach()
  endif()
endmacro()
