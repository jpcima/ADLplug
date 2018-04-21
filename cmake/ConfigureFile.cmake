# ConfigureFile.cmake -- cmake -P script to run configure_file

if(NOT CONFIGURE_FILE_INPUT)
  message(FATAL_ERROR "required variable CONFIGURE_FILE_INPUT not given")
endif()
if(NOT CONFIGURE_FILE_OUTPUT)
  message(FATAL_ERROR "required variable CONFIGURE_FILE_OUTPUT not given")
endif()
configure_file("${CONFIGURE_FILE_INPUT}" "${CONFIGURE_FILE_OUTPUT}")
