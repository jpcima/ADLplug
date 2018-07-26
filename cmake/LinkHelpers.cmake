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

macro(static_link_mingw_crt TARGET)
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND MINGW)
    # forces the static link of standard libraries
    set_property(TARGET "${TARGET}" APPEND_STRING
      PROPERTY LINK_FLAGS " -static-libgcc -static-libstdc++")
  endif()
endmacro()

macro(target_link_static_threads TARGET)
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND MINGW)
    # use a statically link winpthread
    target_link_libraries("${TARGET}" PRIVATE "-Wl,-Bstatic -lwinpthread -Wl,-Bdynamic")
  else()
    find_package(Threads REQUIRED)
    target_link_libraries("${TARGET}" PRIVATE ${CMAKE_THREAD_LIBS_INIT})
  endif()
endmacro()
