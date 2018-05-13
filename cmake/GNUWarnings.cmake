#
# Warnings for GCC and Clang
#
#
# Usage example:
#   enable_gnu_warning("all")
#   disable_gnu_warning("unused-parameter")
#

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

macro(enable_gnu_warning ID)
  if (CMAKE_C_COMPILER_ID MATCHES GNU OR
      CMAKE_C_COMPILER_ID MATCHES Clang)
    check_c_compiler_flag("-W${ID}" _test)
    if(_test)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W${ID}")
    endif()
  endif()
  if (CMAKE_CXX_COMPILER_ID MATCHES GNU OR
      CMAKE_CXX_COMPILER_ID MATCHES Clang)
    check_cxx_compiler_flag("-W${ID}" _test)
    if(_test)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W${ID}")
    endif()
  endif()
  unset(_test)
endmacro()

macro(disable_gnu_warning ID)
  enable_gnu_warning("no-${ID}")
endmacro()
